/*
 * gamepad_web.js
 *
 * Page agent for game controller navigation in the in-app browser. The native side evaluates this
 * whole file as one script in each document and then drives it through window.__gnhPad, one short
 * script per call. The native side chooses spatial-navigation targets from the snapshot geometry;
 * this script collects that geometry, draws the focus ring, scrolls, and activates controls.
 *
 * Results are plain strings: fields separated by ~, list entries by ;, numbers by ,. Coordinates
 * are CSS pixels relative to the viewport, rounded to whole pixels. A failing call returns
 * E~<message>.
 *
 * Styles are set only through CSSOM, never through style attributes or style elements. Listeners
 * are passive and never cancel or stop events. DOM focus moves only to a text field being edited;
 * links, buttons and selects are tracked by reference.
 */
(function () {
    "use strict";

    if (window.__gnhPad && window.__gnhPad.v === 2) {
        return;
    }

    /* Elements that may be candidates; matchesKind() narrows the generic [tabindex] and [role]. */
    var SELECTOR = "a[href],button,input,select,textarea,summary,[contenteditable],[tabindex],[role]";
    var ROLES = { button: true, link: true, tab: true, menuitem: true, checkbox: true };
    var TEXT_TYPES = { text: true, search: true, email: true, url: true, tel: true, password: true, number: true };
    var SCROLLABLE = { auto: true, scroll: true, overlay: true };
    var MEDIA = "img,svg,video,canvas";
    var MAX_FRAGMENTS = 4;
    var MAX_MEDIA = 16;
    var RING_SHADOW = "0 0 0 2px #1A1A1A, 0 0 0 4px #FFE070, 0 0 12px 4px rgba(255, 168, 0, 0.9)";
    /* How long a smooth scroll started here counts as still moving toward its target. */
    var SMOOTH_MS = 1500;
    /* Longest frame delta applied by velocity scrolling, so a stalled frame does not jump. */
    var MAX_FRAME_MS = 100;
    var POINTER_SLOP = 2;
    var OPTS = { capture: true, passive: true };
    /* Areas: m the article, s a sidebar, o anything else. A sidebar root is an aside, a complementary role, or an
       element with an id or class token naming a sidebar (wiki-sidebar, sidebar-content; not has-sidebar). */
    var SIDE_ROLES = { complementary: true };
    var SIDE_TOKEN = /^(?!has-)(?:.*[-_])?sidebar(?:[-_].*)?$/i;
    var OTHER_TAGS = { HEADER: true, FOOTER: true, NAV: true };
    var OTHER_ROLES = { banner: true, contentinfo: true, navigation: true };
    var FOOTER_ID = /footer/i;
    var MAIN_ROOTS = "main,[role=main]";

    /* Snapshot sequence number and the element of each snapshot entry; only the latest is kept. */
    var seq = 0;
    var items = [];
    /* The focused candidate, tracked by reference; DOM focus is not moved to it. */
    var focusEl = null;
    /* The ring shows only while visible and not hidden; pointer, wheel and key use set hidden. */
    var visible = false;
    var hidden = true;
    var ring = [];
    var lastPointer = null;
    /* Target of the latest smooth scroll started here: { x, y, t }, or null. */
    var smooth = null;
    var vel = { x: 0, y: 0, until: 0, accX: 0, accY: 0, last: -1, raf: 0 };
    var resizeObserver = null;
    var observedBody = null;
    /* The last focus in each area, and the area of the last focus */
    var lastByArea = { m: null, s: null, o: null };
    var lastArea = "m";
    /* The initial focus is below the viewport; showing the ring scrolls it into view. Pointer, wheel and key use
       clear it. */
    var initialPending = false;

    /* ---------------------------------------------------------------- helpers */

    function err(e) {
        var m = (e && e.message) ? e.message : String(e);
        return "E~" + String(m).replace(/[~;\r\n]+/g, " ").slice(0, 200);
    }

    function num(v) {
        var n = Number(v);
        return isFinite(n) ? n : 0;
    }

    function rnd(v) {
        return String(Math.round(v));
    }

    function clamp(v, lo, hi) {
        return v < lo ? lo : (v > hi ? hi : v);
    }

    function now() {
        return (window.performance && performance.now) ? performance.now() : Date.now();
    }

    function toArray(list) {
        return Array.prototype.slice.call(list || []);
    }

    /* Parent element, crossing shadow root boundaries to the host. */
    function parentOf(el) {
        if (el.parentElement) {
            return el.parentElement;
        }
        var p = el.parentNode;
        return (p && p.nodeType === 11 && p.host) ? p.host : null;
    }

    function scrollingEl() {
        return document.scrollingElement || document.documentElement;
    }

    function scrollX() {
        return window.pageXOffset || 0;
    }

    function scrollY() {
        return window.pageYOffset || 0;
    }

    /* Layout viewport size without scrollbars in standards mode. */
    function viewport() {
        var de = document.documentElement;
        var w = window.innerWidth;
        var h = window.innerHeight;
        if (document.compatMode === "CSS1Compat" && de) {
            if (de.clientWidth > 0) {
                w = de.clientWidth;
            }
            if (de.clientHeight > 0) {
                h = de.clientHeight;
            }
        }
        return { w: w, h: h };
    }

    function maxScroll(vp) {
        var se = scrollingEl();
        return {
            x: Math.max(0, se.scrollWidth - vp.w),
            y: Math.max(0, se.scrollHeight - vp.h)
        };
    }

    function toRect(r) {
        return { x: r.left, y: r.top, w: r.width, h: r.height };
    }

    function unionRects(list) {
        var i;
        var l = Infinity;
        var t = Infinity;
        var r = -Infinity;
        var b = -Infinity;
        if (!list.length) {
            return null;
        }
        for (i = 0; i < list.length; i++) {
            l = Math.min(l, list[i].x);
            t = Math.min(t, list[i].y);
            r = Math.max(r, list[i].x + list[i].w);
            b = Math.max(b, list[i].y + list[i].h);
        }
        return { x: l, y: t, w: r - l, h: b - t };
    }

    function contains(outer, inner, tolerance) {
        return inner.x >= outer.x - tolerance && inner.y >= outer.y - tolerance &&
            inner.x + inner.w <= outer.x + outer.w + tolerance &&
            inner.y + inner.h <= outer.y + outer.h + tolerance;
    }

    function rectStr(r) {
        return rnd(r.x) + "," + rnd(r.y) + "," + rnd(r.w) + "," + rnd(r.h);
    }

    function domRectStr(r) {
        return rectStr(toRect(r));
    }

    /* Whether any fragment intersects the viewport extended vertically by reach. */
    function intersects(frags, vp, reach) {
        var i;
        var f;
        for (i = 0; i < frags.length; i++) {
            f = frags[i];
            if (f.y + f.h > -reach && f.y < vp.h + reach && f.x + f.w > 0 && f.x < vp.w) {
                return true;
            }
        }
        return false;
    }

    function isRing(el) {
        return ring.indexOf(el) >= 0;
    }

    /* The active element, descending into open shadow roots. */
    function deepActive() {
        var a = document.activeElement;
        while (a && a.shadowRoot && a.shadowRoot.activeElement) {
            a = a.shadowRoot.activeElement;
        }
        return a;
    }

    function isTextInput(el) {
        return !!el && el.localName === "input" && TEXT_TYPES[el.type] === true;
    }

    /* Text-like input, textarea or contenteditable. */
    function isTextField(el) {
        if (!el || el.nodeType !== 1) {
            return false;
        }
        if (el.localName === "textarea") {
            return true;
        }
        if (el.localName === "input") {
            return TEXT_TYPES[el.type] === true;
        }
        return el.isContentEditable === true;
    }

    function isEditing() {
        return isTextField(deepActive());
    }

    function liveFocus() {
        return (focusEl && focusEl.isConnected) ? focusEl : null;
    }

    function firstRole(el) {
        var role = el.getAttribute("role");
        return role ? role.trim().split(/\s+/)[0].toLowerCase() : "";
    }

    /* Kind letter: a link, b button or other control, t single-line text, m multiple-line text,
       s select, u summary. */
    function kindOf(el) {
        var n = el.localName;
        if (n === "a" && el.hasAttribute("href")) {
            return "a";
        }
        if (n === "select") {
            return "s";
        }
        if (n === "textarea") {
            return "m";
        }
        if (n === "input") {
            return TEXT_TYPES[el.type] === true ? "t" : "b";
        }
        if (n === "summary") {
            return "u";
        }
        if (n === "button") {
            return "b";
        }
        if (el.isContentEditable === true) {
            return "m";
        }
        if (firstRole(el) === "link") {
            return "a";
        }
        return "b";
    }

    /* Whether the element is of a candidate kind, before the visibility filters. */
    function matchesKind(el) {
        var n = el.localName;
        var t;
        if (n === "a" && el.hasAttribute("href")) {
            return true;
        }
        if (n === "button" || n === "select" || n === "textarea" || n === "summary") {
            return true;
        }
        if (n === "input") {
            return el.type !== "hidden";
        }
        if (el.hasAttribute("contenteditable") && el.isContentEditable === true) {
            return true;
        }
        if (ROLES[firstRole(el)] === true) {
            return true;
        }
        if (el.hasAttribute("tabindex")) {
            t = parseInt(el.getAttribute("tabindex"), 10);
            return !isNaN(t) && t >= 0;
        }
        return false;
    }

    function summaryOf(details) {
        var c = details.firstElementChild;
        while (c) {
            if (c.localName === "summary") {
                return c;
            }
            c = c.nextElementSibling;
        }
        return null;
    }

    /* Inside a closed details element, except within that element's own summary. */
    function inClosedDetails(el) {
        var child = el;
        var p = el.parentElement;
        while (p) {
            if (p.localName === "details" && !p.open && summaryOf(p) !== child) {
                return true;
            }
            child = p;
            p = p.parentElement;
        }
        return false;
    }

    /* Accessible name of a link: text, an img with alt text, aria-label or aria-labelledby. */
    function hasName(el) {
        var imgs;
        var i;
        if ((el.getAttribute("aria-label") || "").trim() || (el.getAttribute("aria-labelledby") || "").trim()) {
            return true;
        }
        if ((el.textContent || "").trim()) {
            return true;
        }
        imgs = el.querySelectorAll("img[alt]");
        for (i = 0; i < imgs.length; i++) {
            if ((imgs[i].getAttribute("alt") || "").trim()) {
                return true;
            }
        }
        return false;
    }

    /*
     * Viewport rectangles of an element: its non-empty client rectangles, or their bounding
     * rectangle when there are more than four. When it holds images or other media that reach
     * outside its own rectangles (an inline link around a picture), one rectangle covering both.
     */
    function fragments(el) {
        var list = el.getClientRects();
        var out = [];
        var media;
        var bounds;
        var all;
        var outside = false;
        var i;
        var r;
        for (i = 0; i < list.length; i++) {
            r = list[i];
            if (r.width > 0 && r.height > 0) {
                out.push(toRect(r));
            }
        }
        media = el.querySelectorAll ? el.querySelectorAll(MEDIA) : [];
        if (media.length) {
            bounds = unionRects(out);
            all = out.slice();
            for (i = 0; i < media.length && i < MAX_MEDIA; i++) {
                r = media[i].getBoundingClientRect();
                if (r.width > 0 && r.height > 0) {
                    r = toRect(r);
                    all.push(r);
                    if (!bounds || !contains(bounds, r, 2)) {
                        outside = true;
                    }
                }
            }
            if (outside) {
                return [unionRects(all)];
            }
        }
        if (out.length > MAX_FRAGMENTS) {
            return [toRect(el.getBoundingClientRect())];
        }
        return out;
    }

    function hasFragmentIn(el, vp) {
        return intersects(fragments(el), vp, 0);
    }

    /* Fragments of an element that passes every candidate filter, else null. */
    function candidateFragments(el) {
        var frags;
        var cs;
        if (isRing(el) || el.disabled === true) {
            return null;
        }
        try {
            if (el.matches(":disabled")) {
                return null;
            }
        } catch (e) {
            /* :disabled not applicable */
        }
        if (el.closest("[inert]") || el.closest("[aria-hidden=\"true\"]") || inClosedDetails(el)) {
            return null;
        }
        if (kindOf(el) === "a" && !hasName(el)) {
            return null;
        }
        frags = fragments(el);
        if (!frags.length) {
            return null;
        }
        cs = window.getComputedStyle(el);
        if (cs.visibility === "hidden" || cs.visibility === "collapse" || !(parseFloat(cs.opacity) > 0)) {
            return null;
        }
        return frags;
    }

    /* All candidates in DOM order as { el, frags }; a match nested in a kept candidate is dropped. */
    function collectAll() {
        var nodes = document.querySelectorAll(SELECTOR);
        var out = [];
        var outer = null;
        var i;
        var el;
        var frags;
        for (i = 0; i < nodes.length; i++) {
            el = nodes[i];
            if (outer && outer.contains(el)) {
                continue;
            }
            if (!matchesKind(el)) {
                continue;
            }
            frags = candidateFragments(el);
            if (!frags) {
                continue;
            }
            outer = el;
            out.push({ el: el, frags: frags });
        }
        return out;
    }

    /* The outermost candidate at or around a node. */
    function candidateFrom(node) {
        var el = (node && node.nodeType === 1) ? node : (node ? node.parentElement : null);
        var best = null;
        while (el && el.nodeType === 1) {
            if (matchesKind(el) && candidateFragments(el)) {
                best = el;
            }
            el = parentOf(el);
        }
        return best;
    }

    /* Top visible: the first fragment starting highest within the viewport, else the first one
       overlapping it. */
    function topVisible(all, vp) {
        var best = null;
        var bestTop = Infinity;
        var first = null;
        var i;
        var t;
        for (i = 0; i < all.length; i++) {
            if (!intersects(all[i].frags, vp, 0)) {
                continue;
            }
            if (!first) {
                first = all[i];
            }
            t = all[i].frags[0].y;
            if (t >= -1 && t < vp.h && t < bestTop) {
                best = all[i];
                bestTop = t;
            }
        }
        return best || first;
    }

    function contentRoot() {
        return document.querySelector("#wiki-content") || document.querySelector("main") ||
            document.querySelector("article") || document.querySelector("[role=main]");
    }

    function hasSideToken(el) {
        var cls = typeof el.className === "string" ? el.className : "";
        var tokens = cls.split(/\s+/);
        var i;
        if (el.id && SIDE_TOKEN.test(el.id)) {
            return true;
        }
        for (i = 0; i < tokens.length; i++) {
            if (tokens[i] && SIDE_TOKEN.test(tokens[i])) {
                return true;
            }
        }
        return false;
    }

    /* The article root: main or [role=main], else the content root. */
    function mainRoot() {
        return document.querySelector(MAIN_ROOTS) || contentRoot();
    }

    /* The area of an element: inside a sidebar root s; else inside a header, footer or navigation root o; else
       inside the article root m; else o. */
    function areaOf(el, main) {
        var e = el;
        var other = false;
        var role;
        while (e && e.nodeType === 1) {
            role = firstRole(e);
            if (e.tagName === "ASIDE" || SIDE_ROLES[role] || hasSideToken(e)) {
                return "s";
            }
            if (OTHER_TAGS[e.tagName] || OTHER_ROLES[role] || (e.id && FOOTER_ID.test(e.id))) {
                other = true;
            }
            e = parentOf(e);
        }
        if (other) {
            return "o";
        }
        if (main === undefined) {
            main = mainRoot();
        }
        return main && main.contains(el) ? "m" : "o";
    }

    /* Candidates of area a */
    function inArea(all, a) {
        var main = mainRoot();
        return all.filter(function (c) {
            return areaOf(c.el, main) === a;
        });
    }

    function rememberFocus() {
        var a;
        if (focusEl && focusEl.isConnected) {
            a = areaOf(focusEl);
            lastByArea[a] = focusEl;
            lastArea = a;
        }
    }

    function isFixed(el) {
        while (el && el.nodeType === 1) {
            if (window.getComputedStyle(el).position === "fixed") {
                return true;
            }
            el = parentOf(el);
        }
        return false;
    }

    /* ---------------------------------------------------------------- scrolling */

    /* The body's overflow applies to the viewport, so the body is not a scroll container. */
    function bodyPropagates() {
        var cs = window.getComputedStyle(document.documentElement);
        return cs.overflowX === "visible" && cs.overflowY === "visible";
    }

    function isRootScroller(el) {
        return el === document.documentElement || el === scrollingEl() ||
            (el === document.body && bodyPropagates());
    }

    /* Scrollable axes of a scroll container other than the document's, or null. */
    function scrollAxes(el) {
        var cs;
        var x;
        var y;
        if (isRootScroller(el)) {
            return null;
        }
        cs = window.getComputedStyle(el);
        x = SCROLLABLE[cs.overflowX] === true && el.scrollWidth > el.clientWidth;
        y = SCROLLABLE[cs.overflowY] === true && el.scrollHeight > el.clientHeight;
        return (x || y) ? { x: x, y: y, rtl: cs.direction === "rtl" } : null;
    }

    /* Whether the container has room to move along axis in the direction of d. */
    function canScroll(el, axis, d) {
        var a = scrollAxes(el);
        var max;
        var pos;
        if (!a || !a[axis]) {
            return false;
        }
        if (axis === "y") {
            max = el.scrollHeight - el.clientHeight;
            pos = el.scrollTop;
        } else {
            max = el.scrollWidth - el.clientWidth;
            pos = el.scrollLeft;
            if (a.rtl && pos <= 0) {
                /* Right-to-left containers scroll from 0 to a negative scrollLeft. */
                pos += max;
            }
        }
        return d > 0 ? pos < max - 1 : pos > 0;
    }

    /*
     * Scroll target for one axis: the nearest scroll container at or above the focus that can move
     * that way, else null for the document. Without a focus, the search starts from the element at
     * the viewport center.
     */
    function scrollTargetFor(axis, d) {
        var el = liveFocus();
        var vp;
        if (!el) {
            vp = viewport();
            el = document.elementFromPoint(vp.w / 2, vp.h / 2);
        }
        while (el && el.nodeType === 1) {
            if (canScroll(el, axis, d)) {
                return el;
            }
            el = parentOf(el);
        }
        return null;
    }

    function windowBy(dx, dy) {
        try {
            window.scrollBy({ left: dx, top: dy, behavior: "instant" });
        } catch (e) {
            window.scrollBy(dx, dy);
        }
    }

    function elementBy(el, dx, dy) {
        try {
            el.scrollBy({ left: dx, top: dy, behavior: "instant" });
        } catch (e) {
            el.scrollLeft += dx;
            el.scrollTop += dy;
        }
    }

    function smoothTo(x, y) {
        smooth = { x: x, y: y, t: now() };
        try {
            window.scrollTo({ left: x, top: y, behavior: "smooth" });
        } catch (e) {
            window.scrollTo(x, y);
        }
    }

    /* Vertical scroll position a smooth scroll started here is moving to, else the current one. */
    function targetY() {
        var y = scrollY();
        if (smooth && now() - smooth.t < SMOOTH_MS && Math.abs(smooth.y - y) > 1) {
            return smooth.y;
        }
        smooth = null;
        return y;
    }

    /* Scrolls one axis of its scroll target by d CSS px; true if it moved. */
    function scrollAxis(axis, d) {
        var t = scrollTargetFor(axis, d);
        var before;
        var after;
        if (t) {
            before = axis === "x" ? t.scrollLeft : t.scrollTop;
            elementBy(t, axis === "x" ? d : 0, axis === "y" ? d : 0);
            after = axis === "x" ? t.scrollLeft : t.scrollTop;
        } else {
            before = axis === "x" ? scrollX() : scrollY();
            windowBy(axis === "x" ? d : 0, axis === "y" ? d : 0);
            after = axis === "x" ? scrollX() : scrollY();
        }
        return after !== before;
    }

    function moveBy(dx, dy) {
        var moved = false;
        if (dx) {
            moved = scrollAxis("x", dx) || moved;
        }
        if (dy) {
            moved = scrollAxis("y", dy) || moved;
        }
        return moved;
    }

    /* Distance to scroll so a span is inside [lo, lo + len], with 10 % margin at the edge it comes
       in from; a span too large for the margins is aligned to the start margin. */
    function edgeDelta(start, size, lo, len) {
        var m = 0.1 * len;
        var end = start + size;
        if (start >= lo && end <= lo + len) {
            return 0;
        }
        if (start < lo || size > len - 2 * m) {
            return Math.round(start - (lo + m));
        }
        return Math.round(end - (lo + len - m));
    }

    function elementRect(el) {
        return unionRects(fragments(el)) || toRect(el.getBoundingClientRect());
    }

    /* Instantly scrolls an element into its scroll containers and then into the viewport. */
    function reveal(el) {
        var p = parentOf(el);
        var a;
        var r;
        var b;
        var dx;
        var dy;
        var vp;
        while (p && p.nodeType === 1) {
            a = scrollAxes(p);
            if (a) {
                r = elementRect(el);
                b = p.getBoundingClientRect();
                dx = a.x ? edgeDelta(r.x, r.w, b.left + p.clientLeft, p.clientWidth) : 0;
                dy = a.y ? edgeDelta(r.y, r.h, b.top + p.clientTop, p.clientHeight) : 0;
                if (dx || dy) {
                    elementBy(p, dx, dy);
                }
            }
            p = parentOf(p);
        }
        vp = viewport();
        r = elementRect(el);
        dx = edgeDelta(r.x, r.w, 0, vp.w);
        dy = edgeDelta(r.y, r.h, 0, vp.h);
        if (dx || dy) {
            smooth = null;
            windowBy(dx, dy);
        }
    }

    function stopVelocity() {
        vel.x = 0;
        vel.y = 0;
        vel.accX = 0;
        vel.accY = 0;
        vel.last = -1;
        if (vel.raf) {
            window.cancelAnimationFrame(vel.raf);
            vel.raf = 0;
        }
    }

    /* One velocity scrolling frame; whole pixels are applied and the remainder carried over. */
    function velocityFrame(ts) {
        var dt;
        var mx;
        var my;
        vel.raf = 0;
        try {
            if (ts >= vel.until || (!vel.x && !vel.y)) {
                stopVelocity();
                placeRing();
                return;
            }
            dt = vel.last < 0 ? 1000 / 60 : clamp(ts - vel.last, 0, MAX_FRAME_MS);
            vel.last = ts;
            vel.accX += vel.x * dt / 1000;
            vel.accY += vel.y * dt / 1000;
            mx = vel.accX < 0 ? Math.ceil(vel.accX) : Math.floor(vel.accX);
            my = vel.accY < 0 ? Math.ceil(vel.accY) : Math.floor(vel.accY);
            vel.accX -= mx;
            vel.accY -= my;
            if (mx || my) {
                moveBy(mx, my);
            }
            placeRing();
        } catch (e) {
            /* keep scrolling until ttl expires */
        }
        vel.raf = window.requestAnimationFrame(velocityFrame);
    }

    /* ---------------------------------------------------------------- ring */

    function styleRing(d) {
        var s = d.style;
        var props = [
            ["position", "fixed"], ["display", "none"], ["box-sizing", "border-box"], ["margin", "0"],
            ["padding", "0"], ["border", "0"], ["outline", "0"], ["background", "transparent"],
            ["pointer-events", "none"], ["z-index", "2147483647"], ["border-radius", "4px"],
            ["box-shadow", RING_SHADOW], ["transform", "none"], ["transition", "none"],
            ["animation", "none"], ["opacity", "1"], ["visibility", "visible"], ["right", "auto"],
            ["bottom", "auto"], ["min-width", "0"], ["min-height", "0"], ["max-width", "none"],
            ["max-height", "none"]
        ];
        var i;
        for (i = 0; i < props.length; i++) {
            s.setProperty(props[i][0], props[i][1], "important");
        }
        d.setAttribute("aria-hidden", "true");
    }

    function ringDiv(i) {
        var d = ring[i];
        var root = document.documentElement;
        if (!d) {
            d = document.createElement("div");
            styleRing(d);
            ring[i] = d;
        }
        if (d.parentNode !== root) {
            root.appendChild(d);
        }
        return d;
    }

    /* One fixed div per fragment of the focus; hidden without a focus, a fragment, or visibility. */
    function placeRing() {
        var f;
        var frags = [];
        var i;
        var d;
        var s;
        try {
            f = liveFocus();
            if (f && visible && !hidden) {
                frags = fragments(f);
            }
            for (i = 0; i < MAX_FRAGMENTS; i++) {
                if (i < frags.length) {
                    d = ringDiv(i);
                    s = d.style;
                    s.setProperty("left", frags[i].x + "px", "important");
                    s.setProperty("top", frags[i].y + "px", "important");
                    s.setProperty("width", frags[i].w + "px", "important");
                    s.setProperty("height", frags[i].h + "px", "important");
                    s.setProperty("display", "block", "important");
                } else if (ring[i]) {
                    ring[i].style.setProperty("display", "none", "important");
                }
            }
        } catch (e) {
            /* the ring is cosmetic */
        }
    }

    function observeLayout() {
        if (resizeObserver || typeof window.ResizeObserver !== "function") {
            if (resizeObserver && document.body && observedBody !== document.body) {
                observedBody = document.body;
                resizeObserver.observe(observedBody);
            }
            return;
        }
        resizeObserver = new window.ResizeObserver(function () {
            placeRing();
        });
        resizeObserver.observe(document.documentElement);
        if (document.body) {
            observedBody = document.body;
            resizeObserver.observe(observedBody);
        }
    }

    /* ---------------------------------------------------------------- input listeners */

    function hideByInput() {
        hidden = true;
        smooth = null;
        initialPending = false;
        placeRing();
    }

    function onPointerDown(e) {
        var c;
        try {
            if (!e.isTrusted) {
                return;
            }
            lastPointer = { x: e.clientX, y: e.clientY };
            c = candidateFrom(e.target);
            if (c) {
                focusEl = c;
            }
            hideByInput();
        } catch (x) {
            /* listeners never throw into the page */
        }
    }

    /* The first move only records the position: a stationary pointer reports moves after
       scrolling and layout changes. */
    function onPointerMove(e) {
        try {
            if (!e.isTrusted) {
                return;
            }
            if (!lastPointer) {
                lastPointer = { x: e.clientX, y: e.clientY };
                return;
            }
            if (Math.abs(e.clientX - lastPointer.x) > POINTER_SLOP ||
                Math.abs(e.clientY - lastPointer.y) > POINTER_SLOP) {
                lastPointer = { x: e.clientX, y: e.clientY };
                hideByInput();
            }
        } catch (x) {
            /* listeners never throw into the page */
        }
    }

    function onTouchStart(e) {
        var t = e.touches && e.touches[0];
        if (t) {
            onPointerDown({ isTrusted: e.isTrusted, clientX: t.clientX, clientY: t.clientY, target: e.target });
        }
    }

    function onWheel(e) {
        try {
            if (e.isTrusted) {
                hideByInput();
            }
        } catch (x) {
            /* listeners never throw into the page */
        }
    }

    /* Keys typed into a field being edited, such as from the on-screen keyboard, keep the ring. */
    function onKeyDown(e) {
        try {
            if (e.isTrusted && !isEditing()) {
                hideByInput();
            }
        } catch (x) {
            /* listeners never throw into the page */
        }
    }

    function onLayout() {
        placeRing();
    }

    /* ---------------------------------------------------------------- activation */

    /* Clicks an element; true if a page handler canceled the click. */
    function clickEl(el) {
        var ev = null;
        function record(e) {
            if (!ev) {
                ev = e;
            }
        }
        window.addEventListener("click", record, OPTS);
        try {
            if (typeof el.click === "function") {
                el.click();
            } else {
                el.dispatchEvent(new MouseEvent("click", { bubbles: true, cancelable: true, view: window }));
            }
        } finally {
            window.removeEventListener("click", record, OPTS);
        }
        return ev ? ev.defaultPrevented : false;
    }

    function linkUrl(el) {
        var href = el.getAttribute("href");
        if (href === null) {
            href = el.getAttributeNS("http://www.w3.org/1999/xlink", "href");
        }
        if (href === null) {
            return null;
        }
        try {
            return new URL(href, document.baseURI);
        } catch (e) {
            return null;
        }
    }

    /* g when following the URL loads a document, c for same-document fragments and other schemes. */
    function navKind(url) {
        if (!/^(https?|file):$/.test(url.protocol)) {
            return "c";
        }
        if (url.hash && url.href.split("#")[0] === location.href.split("#")[0]) {
            return "c";
        }
        return "g";
    }

    /*
     * Follows a link with click() so page scripts run. A link to a new browsing context
     * (target="_blank" or a named target) is followed in this view with location.assign().
     */
    function followLink(el) {
        var url;
        var target;
        var base;
        if (el.localName !== "a") {
            clickEl(el);
            return "c";
        }
        url = linkUrl(el);
        if (url && url.protocol === "javascript:") {
            clickEl(el);
            return "c";
        }
        target = el.getAttribute("target");
        if (target === null) {
            base = document.querySelector("base[target]");
            target = base ? base.getAttribute("target") : "";
        }
        target = target.trim().toLowerCase();
        if (target && target !== "_self" && target !== "_top" && target !== "_parent" &&
            url && !el.hasAttribute("download")) {
            location.assign(url.href);
            return navKind(url);
        }
        if (clickEl(el) || !url || el.hasAttribute("download")) {
            return "c";
        }
        return navKind(url);
    }

    function optionEnabled(o) {
        return !o.disabled && !(o.parentNode && o.parentNode.localName === "optgroup" && o.parentNode.disabled);
    }

    /* Steps a select to the next enabled option in direction d, firing input and change;
       true if it changed. */
    function stepSelect(sel, d, wrap) {
        var opts = sel.options;
        var n = opts.length;
        var start = sel.selectedIndex;
        var i = start;
        var k;
        if (!n) {
            return false;
        }
        for (k = 0; k < n; k++) {
            i += d;
            if (i >= n) {
                if (!wrap) {
                    return false;
                }
                i = 0;
            } else if (i < 0) {
                if (!wrap) {
                    return false;
                }
                i = n - 1;
            }
            if (optionEnabled(opts[i])) {
                break;
            }
        }
        if (k === n || i === start) {
            return false;
        }
        sel.selectedIndex = i;
        sel.dispatchEvent(new Event("input", { bubbles: true }));
        sel.dispatchEvent(new Event("change", { bubbles: true }));
        return true;
    }

    function editField(el) {
        reveal(el);
        try {
            el.focus({ preventScroll: true });
        } catch (e) {
            el.focus();
        }
        return "A~" + (kindOf(el) === "m" ? "m" : "t") + "~" + domRectStr(el.getBoundingClientRect());
    }

    /* ---------------------------------------------------------------- search field */

    function usableField(el) {
        return !!el && !el.disabled && !el.readOnly && !el.closest("[inert]") &&
            !el.closest("[aria-hidden=\"true\"]");
    }

    /* Search field candidates in priority order: input[type=search], a text input in
       [role=search], a text input in a form whose action contains search, input[name=q]. */
    function searchFields() {
        var out = [];
        var forms = document.forms;
        var i;
        var j;
        var els;
        function add(el) {
            if (out.indexOf(el) < 0 && usableField(el)) {
                out.push(el);
            }
        }
        toArray(document.querySelectorAll("input[type=search]")).forEach(add);
        toArray(document.querySelectorAll("[role=search] input")).filter(isTextInput).forEach(add);
        for (i = 0; i < forms.length; i++) {
            if ((forms[i].getAttribute("action") || "").toLowerCase().indexOf("search") < 0) {
                continue;
            }
            els = forms[i].elements;
            for (j = 0; j < els.length; j++) {
                if (isTextInput(els[j])) {
                    add(els[j]);
                }
            }
        }
        toArray(document.querySelectorAll("input[name=q]")).filter(isTextInput).forEach(add);
        return out;
    }

    function openDetailsAround(el) {
        var opened = [];
        var p = el.parentElement;
        while (p) {
            if (p.localName === "details" && !p.open) {
                p.open = true;
                opened.push(p);
            }
            p = p.parentElement;
        }
        return opened;
    }

    /* ---------------------------------------------------------------- calls */

    /* snapshot(reach): S~seq~vw~vh~dpr~sx~sy~maxX~maxY~focus~flags~items, items index,kind,area,x,y,w,h....
       A focus that is disconnected or has no fragment in the viewport is reported as lost (l). */
    function snapshot(reach) {
        var vp = viewport();
        var mx = maxScroll(vp);
        var all = collectAll();
        var f = liveFocus();
        var main = mainRoot();
        var parts = [];
        var focusPos = -1;
        var flags = "";
        var i;
        var c;
        reach = Math.max(0, num(reach));
        seq += 1;
        items = [];
        for (i = 0; i < all.length; i++) {
            c = all[i];
            if (!intersects(c.frags, vp, reach)) {
                continue;
            }
            if (c.el === f) {
                focusPos = items.length;
            }
            items.push(c.el);
            parts.push(i + "," + kindOf(c.el) + "," + areaOf(c.el, main) + "," + c.frags.map(rectStr).join(","));
        }
        if (hidden) {
            flags += "h";
        }
        if (focusEl && (!f || !hasFragmentIn(f, vp))) {
            flags += "l";
        }
        if (isEditing()) {
            flags += "e";
        }
        if (initialPending) {
            flags += "p";
        }
        return "S~" + seq + "~" + rnd(vp.w) + "~" + rnd(vp.h) + "~" + (window.devicePixelRatio || 1).toFixed(2) +
            "~" + rnd(scrollX()) + "~" + rnd(scrollY()) + "~" + rnd(mx.x) + "~" + rnd(mx.y) +
            "~" + focusPos + "~" + flags + "~" + parts.join(";");
    }

    /* focus(seq, pos): sets the focus whether or not the ring is shown; leaves DOM focus alone. */
    function focusAt(s, pos) {
        var el;
        if (num(s) !== seq) {
            return "X";
        }
        el = items[num(pos)];
        if (!el || !el.isConnected) {
            return "X";
        }
        focusEl = el;
        reveal(el);
        return "F~" + kindOf(el);
    }

    /* activate(): selects step forward with wrapping; text fields are DOM-focused for editing. */
    function activate() {
        var f = liveFocus();
        var k;
        if (!f) {
            return "A~n";
        }
        k = kindOf(f);
        if (k === "s") {
            stepSelect(f, 1, true);
            return "A~s";
        }
        if (k === "t" || k === "m") {
            return editField(f);
        }
        if (k === "a") {
            return "A~" + followLink(f);
        }
        clickEl(f);
        return "A~c";
    }

    /* adjust(dir): steps the focused select, stopping at the ends. */
    function adjust(dir) {
        var f = liveFocus();
        if (!f || kindOf(f) !== "s") {
            return "D~0";
        }
        return stepSelect(f, num(dir) < 0 ? -1 : 1, false) ? "D~1" : "D~0";
    }

    /* endEdit(): blurs the field being edited and makes it the focus when it is a candidate. */
    function endEdit() {
        var a = deepActive();
        if (!isTextField(a)) {
            return "D~0";
        }
        a.blur();
        if (matchesKind(a) && candidateFragments(a)) {
            focusEl = a;
        }
        return "D~1";
    }

    /* closeLayer(): closes the nearest open details containing the focus, its own summary
       included, and focuses that summary. */
    function closeLayer() {
        var f = liveFocus();
        var d;
        var s;
        if (!f) {
            return "D~0";
        }
        d = f.closest("details[open]");
        if (!d) {
            return "D~0";
        }
        d.open = false;
        s = summaryOf(d);
        focusEl = (s && candidateFragments(s)) ? s : null;
        if (focusEl) {
            reveal(focusEl);
        }
        return "D~1";
    }

    /* scroll(vx, vy, ttl): velocity in CSS px/s until ttl ms from now; 0, 0 stops. */
    function scroll(vx, vy, ttl) {
        vx = num(vx);
        vy = num(vy);
        smooth = null;
        if (!vx && !vy) {
            stopVelocity();
            return "D~1";
        }
        if ((vx < 0) !== (vel.x < 0) || !vx) {
            vel.accX = 0;
        }
        if ((vy < 0) !== (vel.y < 0) || !vy) {
            vel.accY = 0;
        }
        vel.x = vx;
        vel.y = vy;
        vel.until = now() + Math.max(0, num(ttl));
        if (!vel.raf) {
            vel.last = -1;
            vel.raf = window.requestAnimationFrame(velocityFrame);
        }
        return "D~1";
    }

    /* step(dx, dy): instant scroll of the scroll target. */
    function step(dx, dy) {
        smooth = null;
        return moveBy(Math.round(num(dx)), Math.round(num(dy))) ? "D~1" : "D~0";
    }

    /* page(dir): smooth page scroll, counted from the target of a smooth scroll still under way. */
    function page(dir) {
        var vp = viewport();
        var mx = maxScroll(vp);
        var y = clamp(targetY() + (num(dir) < 0 ? -1 : 1) * 0.85 * vp.h, 0, mx.y);
        smoothTo(scrollX(), y);
        return "D~1";
    }

    /* top(): smooth scroll to the top; fixed-position candidates are measured where they stand. */
    function top() {
        var vp = viewport();
        var sx = scrollX();
        var sy = scrollY();
        var all = collectAll();
        var pick = null;
        var i;
        var r;
        smoothTo(0, 0);
        for (i = 0; i < all.length && !pick; i++) {
            r = all[i].frags[0];
            if (r.y + sy < vp.h && r.y + r.h + sy > 0 && r.x + sx < vp.w && r.x + r.w + sx > 0) {
                pick = all[i].el;
            } else if (r.y < vp.h && r.y + r.h > 0 && isFixed(all[i].el)) {
                pick = all[i].el;
            }
        }
        focusEl = pick;
        return pick ? "F~" + kindOf(pick) : "F~n";
    }

    /*
     * section(dir): smooth-scrolls the next or previous content heading to the top, measured from
     * the target of a smooth scroll still under way. Without such a heading nothing changes and the
     * current focus is reported.
     */
    function section(dir) {
        var root = contentRoot() || document;
        var hs = root.querySelectorAll("h1,h2,h3");
        var vp = viewport();
        var mx = maxScroll(vp);
        var sy = scrollY();
        var ref = targetY();
        var heading = null;
        var headTop = 0;
        var all;
        var pick = null;
        var i;
        var r;
        var t;
        var f;
        for (i = 0; i < hs.length; i++) {
            r = hs[i].getBoundingClientRect();
            if (!(r.width > 0 || r.height > 0) || inClosedDetails(hs[i])) {
                continue;
            }
            t = r.top + sy;
            if (num(dir) < 0) {
                if (t < ref - 1) {
                    heading = hs[i];
                    headTop = t;
                }
            } else if (t > ref + 1) {
                heading = hs[i];
                headTop = t;
                break;
            }
        }
        if (!heading) {
            f = liveFocus();
            return f ? "F~" + kindOf(f) : "F~n";
        }
        all = collectAll();
        smoothTo(scrollX(), clamp(headTop, 0, mx.y));
        for (i = 0; i < all.length; i++) {
            if (heading.compareDocumentPosition(all[i].el) & 4) {
                t = all[i].frags[0].y + sy;
                if (t >= headTop - 1 && t < headTop + vp.h) {
                    pick = all[i].el;
                }
                break;
            }
        }
        focusEl = pick;
        return pick ? "F~" + kindOf(pick) : "F~n";
    }

    /* search(): the first search field that is shown, or is shown once the closed details around
       it are opened; details opened for a field that stays hidden are closed again. */
    function search() {
        var fields = searchFields();
        var i;
        var el;
        var opened;
        var j;
        for (i = 0; i < fields.length; i++) {
            el = fields[i];
            if (!fragments(el).length) {
                if (!inClosedDetails(el)) {
                    continue;
                }
                opened = openDetailsAround(el);
                if (!fragments(el).length) {
                    for (j = 0; j < opened.length; j++) {
                        opened[j].open = false;
                    }
                    continue;
                }
            }
            focusEl = el;
            return editField(el);
        }
        return "A~n";
    }

    /* The top visible candidate of area a, else of any area */
    function topVisibleIn(all, vp, a) {
        return topVisible(inArea(all, a), vp) || topVisible(all, vp);
    }

    /* initial(): the first article candidate in the viewport; else the first one starting within the next
       viewport height below it, pending until the ring shows; else the top visible preferring the article. */
    function initial() {
        var vp = viewport();
        var all = collectAll();
        var article = inArea(all, "m");
        var pick = null;
        var i;
        var t;
        initialPending = false;
        for (i = 0; i < article.length; i++) {
            if (intersects(article[i].frags, vp, 0)) {
                pick = article[i];
                break;
            }
        }
        if (!pick) {
            for (i = 0; i < article.length; i++) {
                t = article[i].frags[0].y;
                if (t >= vp.h && t < 2 * vp.h) {
                    pick = article[i];
                    initialPending = true;
                    break;
                }
            }
        }
        if (!pick) {
            pick = topVisibleIn(all, vp, "m");
        }
        focusEl = pick ? pick.el : null;
        return pick ? "F~" + kindOf(pick.el) : "F~n";
    }

    /* setVisible(b): showing clears h, and moves a missing focus, or one with no fragment in the
       viewport, to the top visible candidate of the last focus's area; hiding sets h. */
    function setVisible(b) {
        var vp;
        var f;
        var tv;
        if (b && b !== "0" && b !== "false") {
            visible = true;
            hidden = false;
            vp = viewport();
            f = liveFocus();
            if (initialPending && f) {
                initialPending = false;
                reveal(f);
            } else if (!f || !hasFragmentIn(f, vp)) {
                tv = topVisibleIn(collectAll(), vp, f ? areaOf(f) : lastArea);
                if (tv) {
                    focusEl = tv.el;
                }
            }
        } else {
            visible = false;
            hidden = true;
        }
        return "D~1";
    }

    /* hasSearch(): a search field that is shown or sits in closed details. */
    function hasSearch() {
        var fields = searchFields();
        var i;
        for (i = 0; i < fields.length; i++) {
            if (fragments(fields[i]).length || inClosedDetails(fields[i])) {
                return "D~1";
            }
        }
        return "D~0";
    }

    /* area(a): focuses area a: its last focus if still a candidate in the viewport, else its top visible
       candidate, else its first candidate, scrolled into view. */
    function area(a) {
        var vp = viewport();
        var all = collectAll();
        var cands;
        var last;
        var pick;
        var i;
        a = String(a);
        last = lastByArea[a];
        if (last && last.isConnected && areaOf(last) === a) {
            for (i = 0; i < all.length; i++) {
                if (all[i].el === last && intersects(all[i].frags, vp, 0)) {
                    focusEl = last;
                    return "F~" + kindOf(last);
                }
            }
        }
        cands = inArea(all, a);
        if (!cands.length) {
            return "F~n";
        }
        pick = topVisible(cands, vp) || cands[0];
        focusEl = pick.el;
        reveal(pick.el);
        return "F~" + kindOf(pick.el);
    }

    /* hasArea(a): whether area a has a candidate. */
    function hasArea(a) {
        return inArea(collectAll(), String(a)).length ? "D~1" : "D~0";
    }

    /* Wraps a call: exceptions become E~<message>; afterward the focus's area is remembered and the ring
       re-placed. */
    function guard(fn) {
        return function () {
            try {
                observeLayout();
                return fn.apply(null, arguments);
            } catch (e) {
                return err(e);
            } finally {
                try {
                    rememberFocus();
                } catch (e2) {
                    /* the area memory is a convenience */
                }
                placeRing();
            }
        };
    }

    if (window.PointerEvent) {
        window.addEventListener("pointerdown", onPointerDown, OPTS);
        window.addEventListener("pointermove", onPointerMove, OPTS);
    } else {
        window.addEventListener("mousedown", onPointerDown, OPTS);
        window.addEventListener("mousemove", onPointerMove, OPTS);
        window.addEventListener("touchstart", onTouchStart, OPTS);
    }
    window.addEventListener("wheel", onWheel, OPTS);
    window.addEventListener("keydown", onKeyDown, OPTS);
    window.addEventListener("resize", onLayout, OPTS);
    document.addEventListener("scroll", onLayout, OPTS);
    try {
        observeLayout();
    } catch (e) {
        /* the ring is still re-placed after calls and on scroll */
    }

    window.__gnhPad = {
        v: 2,
        snapshot: guard(snapshot),
        focus: guard(focusAt),
        activate: guard(activate),
        adjust: guard(adjust),
        endEdit: guard(endEdit),
        closeLayer: guard(closeLayer),
        scroll: guard(scroll),
        step: guard(step),
        page: guard(page),
        top: guard(top),
        section: guard(section),
        search: guard(search),
        initial: guard(initial),
        setVisible: guard(setVisible),
        hasSearch: guard(hasSearch),
        area: guard(area),
        hasArea: guard(hasArea)
    };
})();
