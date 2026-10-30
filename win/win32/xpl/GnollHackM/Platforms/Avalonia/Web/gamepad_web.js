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
    /* Layers: a modal dialog, a menu popover, or a listbox. */
    var LAYER_ROOTS = "dialog,[popover],[role=listbox]";
    var LISTBOX = "[role=listbox]";
    var OPTION = "[role=option]";
    var MENU_POPOVERS = { "": true, auto: true };
    /* Page hint attributes */
    var AREA_ATTR = "data-gnh-pad-area";
    var SCROLL_MARK = "[data-gnh-pad-scroll]";
    var INITIAL_MARK = "[data-gnh-pad-initial]";
    var SECTION_ATTR = "data-gnh-pad-section";
    var KEY_ATTR = "data-gnh-pad-key";
    var NOCLOSE_ATTR = "data-gnh-pad-noclose";
    var LAYER_MARK = "[data-gnh-pad-layer]";
    var VALID_AREAS = { m: true, s: true, o: true };
    /* Zones and rows: the nearest data-gnh-pad-zone names the zone; a row's candidates are one stop, except those
       in its secondary part. The element with the ring carries data-gnh-pad-focused. */
    var ZONE_ATTR = "data-gnh-pad-zone";
    var ROW_MARK = "[data-gnh-pad-row]";
    var SECONDARY_MARK = "[data-gnh-pad-secondary]";
    var FOCUSED_ATTR = "data-gnh-pad-focused";
    /* In zone mode, the element the keys y, lt and rt go to while it is in the current zone; a mark on an element that
       is not a candidate applies to the first candidate inside it. */
    var SHORTCUT_ATTR = "data-gnh-pad-shortcut";
    /* Largest gap or overlap, in CSS px, between the boxes of zones stacked so that Down or Up hands off */
    var NEIGHBOR_GAP = 8;
    /* Values of will-change and contain that make an element the containing block of fixed-position descendants */
    var FIXED_WILL_CHANGE = /\b(?:transform|perspective|filter)\b/;
    var FIXED_CONTAIN = /\b(?:paint|layout|strict|content)\b/;
    /* Attributes whose changes re-place the ring while it shows */
    var WATCHED_ATTRS = ["open", "hidden", "style", "class", "popover", "aria-expanded"];

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
    /* Target of the latest smooth scroll started here: { el, x, y, t }, el null for the document, or null. */
    var smooth = null;
    /* Velocity scrolling; inner caches the inner scroller of each axis for the session and its focus. */
    var vel = { x: 0, y: 0, until: 0, accX: 0, accY: 0, last: -1, raf: 0, inner: {}, focus: null };
    /* While a frame runs, inner scrollers come from vel.inner. */
    var inFrame = false;
    var resizeObserver = null;
    var observedBody = null;
    /* The last focus in each area, and the area of the last focus */
    var lastByArea = { m: null, s: null, o: null };
    var lastArea = "m";
    /* The initial focus is below the viewport; showing the ring scrolls it into view. Pointer, wheel and key use
       clear it. */
    var initialPending = false;
    /* The focus left outside each open layer when it was first seen: { layer, el }; closing the layer restores it. */
    var origins = [];
    var MAX_ORIGINS = 8;
    /* The active layer, cached within one public call or velocity frame while layerCaching is non-zero;
       undefined when not yet computed. */
    var layerCache;
    var layerCaching = 0;
    /* A listbox sent Escape by closeLayer() that was still shown afterward */
    var pendingClose = null;
    /* Running opacity transitions, cached with the active layer; undefined when not yet computed. */
    var fadeCache;
    /* The last focus in each zone by number, the current zone (0 for none), and the box the ring outlines while
       the current zone has no candidate */
    var lastByZone = {};
    var curZone = 0;
    var zoneBox = null;
    /* The element carrying data-gnh-pad-focused, and the last style written to each ring div */
    var marked = null;
    var ringKeys = [];
    /* Mutations re-place the ring, at most once per frame, while it shows. */
    var mutationObserver = null;
    var watching = false;
    var ringRaf = 0;

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

    /* The intersection of two rectangles, or null when it is empty. */
    function intersectRects(a, b) {
        var l = Math.max(a.x, b.x);
        var t = Math.max(a.y, b.y);
        var r = Math.min(a.x + a.w, b.x + b.w);
        var e = Math.min(a.y + a.h, b.y + b.h);
        return (r > l && e > t) ? { x: l, y: t, w: r - l, h: e - t } : null;
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

    /* The focus while connected and inside the active layer, if any; a focus outside it is recorded as the
       layer's origin. Once no layer is open, a focus that is missing, disconnected or inside a recorded layer
       returns to the first such layer's origin while that is still a candidate, and origins are forgotten. */
    function liveFocus() {
        var layer;
        var o;
        var i;
        if (!origins.length && (!focusEl || !focusEl.isConnected)) {
            return null;
        }
        layer = activeLayer();
        if (!layer && origins.length) {
            for (i = 0; i < origins.length; i++) {
                o = origins[i];
                if ((!focusEl || !focusEl.isConnected || o.layer.contains(focusEl)) && o.el.isConnected &&
                    matchesKind(o.el) && candidateFragments(o.el)) {
                    focusEl = o.el;
                    break;
                }
            }
            origins = [];
        }
        if (!focusEl || !focusEl.isConnected) {
            return null;
        }
        if (layer && !layer.contains(focusEl)) {
            if (originIndex(layer) < 0) {
                origins.push({ layer: layer, el: focusEl });
                if (origins.length > MAX_ORIGINS) {
                    origins.shift();
                }
            }
            return null;
        }
        return focusEl;
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

    /* Whether the element is of a candidate kind, before the visibility filters. Options of a listbox are
       candidates in place of the listbox itself. */
    function matchesKind(el) {
        var n = el.localName;
        var role;
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
        role = firstRole(el);
        if (ROLES[role] === true) {
            return true;
        }
        if (role === "option" && el.closest(LISTBOX)) {
            return true;
        }
        if (role === "listbox" && el.querySelector(OPTION)) {
            return false;
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

    /* Running animations of opacity, transitions included, as { el, show }: show when the last keyframe giving
       opacity gives one above 0; cached with the active layer. */
    function fades() {
        var out = [];
        var list;
        var a;
        var k;
        var end;
        var i;
        var j;
        if (layerCaching && fadeCache !== undefined) {
            return fadeCache;
        }
        if (typeof document.getAnimations === "function") {
            try {
                list = document.getAnimations();
                for (i = 0; i < list.length; i++) {
                    a = list[i];
                    if (a.playState === "finished" || !a.effect || !a.effect.target || a.effect.pseudoElement ||
                        typeof a.effect.getKeyframes !== "function") {
                        continue;
                    }
                    k = a.effect.getKeyframes();
                    end = NaN;
                    for (j = k.length - 1; j >= 0 && isNaN(end); j--) {
                        if (k[j].opacity !== undefined && k[j].opacity !== null) {
                            end = parseFloat(k[j].opacity);
                        }
                    }
                    if (!isNaN(end)) {
                        out.push({ el: a.effect.target, show: end > 0 });
                    }
                }
            } catch (e) {
                out = [];
            }
        }
        if (layerCaching) {
            fadeCache = out;
        }
        return out;
    }

    /* Whether el is shown, opacity 0 counting as hidden when opacity is set: checkVisibility() where available,
       else el's visibility and the display and opacity of el and its ancestors up to the body. */
    function shownBy(el, opacity) {
        var cs;
        var e;
        if (typeof el.checkVisibility === "function") {
            return el.checkVisibility({ opacityProperty: opacity, visibilityProperty: true });
        }
        cs = window.getComputedStyle(el);
        if (cs.visibility === "hidden" || cs.visibility === "collapse") {
            return false;
        }
        for (e = el; e && e.nodeType === 1 && e !== document.documentElement; e = parentOf(e)) {
            if (e !== el) {
                cs = window.getComputedStyle(e);
            }
            if (cs.display === "none" || (opacity && !(parseFloat(cs.opacity) > 0))) {
                return false;
            }
        }
        return true;
    }

    /* Whether nothing hides el: it and its ancestors are not display none or at opacity 0, and its visibility is
       visible. An opacity animation counts by its end value: inside an element fading in el is shown, inside one
       fading out it is not. */
    function effectivelyVisible(el) {
        var list = fades();
        var e;
        var i;
        for (i = 0; i < list.length; i++) {
            if (!list[i].show && list[i].el.contains(el)) {
                return false;
            }
        }
        if (shownBy(el, true)) {
            return true;
        }
        if (!list.length || !shownBy(el, false)) {
            return false;
        }
        for (e = el; e && e.nodeType === 1 && e !== document.documentElement; e = parentOf(e)) {
            if (!(parseFloat(window.getComputedStyle(e).opacity) > 0) && !fadingIn(list, e)) {
                return false;
            }
        }
        return true;
    }

    /* Whether list holds a transition fading el in */
    function fadingIn(list, el) {
        var i;
        for (i = 0; i < list.length; i++) {
            if (list[i].el === el && list[i].show) {
                return true;
            }
        }
        return false;
    }

    /* Fragments of an element that passes every candidate filter, else null. */
    function candidateFragments(el) {
        var frags;
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
        if (!effectivelyVisible(el)) {
            return null;
        }
        return frags;
    }

    /* All candidates in DOM order as { el, frags }; a match nested in a kept candidate is dropped. While a layer
       is open, only the candidates inside it. */
    function collectAll() {
        var layer = activeLayer();
        var nodes = (layer || document).querySelectorAll(SELECTOR);
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

    /* Whether a container holds a candidate */
    function hasCandidate(root) {
        var nodes = root.querySelectorAll(SELECTOR);
        var i;
        for (i = 0; i < nodes.length; i++) {
            if (matchesKind(nodes[i]) && candidateFragments(nodes[i])) {
                return true;
            }
        }
        return false;
    }

    function matchesSafe(el, sel, fallback) {
        try {
            return el.matches(sel);
        } catch (e) {
            return fallback;
        }
    }

    function popoverOpen(el) {
        return matchesSafe(el, ":popover-open", false);
    }

    /* An open dialog shown modally; where :modal is unsupported, any open dialog. */
    function isModal(d) {
        return d.open === true && matchesSafe(d, ":modal", true);
    }

    function isShown(el) {
        var cs;
        if (!el.isConnected || !el.getClientRects().length) {
            return false;
        }
        cs = window.getComputedStyle(el);
        return cs.visibility !== "hidden" && cs.visibility !== "collapse";
    }

    /* The last element of list containing the active element, else the last one, else null. */
    function topmost(list) {
        var a = deepActive();
        var i;
        for (i = list.length - 1; i >= 0; i--) {
            if (a && list[i].contains(a)) {
                return list[i];
            }
        }
        return list.length ? list[list.length - 1] : null;
    }

    /* Elements whose aria-controls or aria-owns names el by id; only those with aria-expanded="true" when
       expanded is set. */
    function controllersOf(el, expanded) {
        var id = el.id;
        var nodes;
        var out = [];
        var i;
        var refs;
        if (!id) {
            return out;
        }
        nodes = document.querySelectorAll("[aria-controls],[aria-owns]");
        for (i = 0; i < nodes.length; i++) {
            if (expanded && nodes[i].getAttribute("aria-expanded") !== "true") {
                continue;
            }
            refs = ((nodes[i].getAttribute("aria-controls") || "") + " " +
                (nodes[i].getAttribute("aria-owns") || "")).split(/\s+/);
            if (refs.indexOf(id) >= 0) {
                out.push(nodes[i]);
            }
        }
        return out;
    }

    /* Whether a listbox shows as a popup: an expanded owner names it, it or an ancestor up to the dialog or
       body is absolutely or fixed positioned, or it or an ancestor carries data-gnh-pad-layer. */
    function isPopupListbox(b, dialog) {
        var e = b;
        var pos;
        if (b.closest(LAYER_MARK) || controllersOf(b, true).length) {
            return true;
        }
        while (e && e.nodeType === 1 && e !== dialog && e !== document.body && e !== document.documentElement) {
            pos = window.getComputedStyle(e).position;
            if (pos === "absolute" || pos === "fixed") {
                return true;
            }
            e = parentOf(e);
        }
        return false;
    }

    /*
     * Open layers, each null when absent: the innermost open menu popover (popover auto or empty) holding a
     * candidate; the last shown popup listbox holding a candidate outside any popover, and inside the topmost
     * modal dialog while one is open; and the topmost modal dialog. Hint and manual popovers are never layers,
     * and listboxes shown as ordinary content are not either.
     */
    function layers() {
        var nodes = document.querySelectorAll(LAYER_ROOTS);
        var out = { popover: null, listbox: null, dialog: null };
        var pops = [];
        var boxes = [];
        var dialogs = [];
        var i;
        var el;
        if (!nodes.length) {
            return out;
        }
        for (i = 0; i < nodes.length; i++) {
            el = nodes[i];
            if (el.localName === "dialog" && isModal(el)) {
                dialogs.push(el);
            }
            if (el.hasAttribute("popover") &&
                MENU_POPOVERS[el.getAttribute("popover").trim().toLowerCase()] === true && popoverOpen(el)) {
                pops.push(el);
            }
            if (firstRole(el) === "listbox" && !el.closest("[popover]")) {
                boxes.push(el);
            }
        }
        out.dialog = topmost(dialogs);
        out.popover = topmost(pops.filter(hasCandidate));
        boxes = boxes.filter(function (b) {
            return (!out.dialog || out.dialog.contains(b)) && isShown(b) && hasCandidate(b) &&
                isPopupListbox(b, out.dialog);
        });
        out.listbox = boxes.length ? boxes[boxes.length - 1] : null;
        return out;
    }

    /* The innermost open layer, or null; cached while layerCaching is non-zero. */
    function activeLayer() {
        var l;
        if (layerCaching && layerCache !== undefined) {
            return layerCache;
        }
        l = layers();
        l = l.popover || l.listbox || l.dialog;
        if (layerCaching) {
            layerCache = l;
        }
        return l;
    }

    function invalidateLayer() {
        layerCache = undefined;
        fadeCache = undefined;
    }

    /* Runs fn with the active layer cached */
    function withLayerCache(fn, args) {
        layerCaching++;
        try {
            return fn.apply(null, args);
        } finally {
            layerCaching--;
            if (!layerCaching) {
                invalidateLayer();
            }
        }
    }

    function originIndex(layer) {
        var i;
        for (i = 0; i < origins.length; i++) {
            if (origins[i].layer === layer) {
                return i;
            }
        }
        return -1;
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

    /* The value of the nearest data-gnh-pad-area naming an area, else null. */
    function markedArea(el) {
        var v;
        while (el && el.nodeType === 1) {
            v = el.getAttribute(AREA_ATTR);
            if (v !== null && VALID_AREAS[v.trim()] === true) {
                return v.trim();
            }
            el = parentOf(el);
        }
        return null;
    }

    /* The area of an element: the area marked by data-gnh-pad-area; else inside a sidebar root s; else inside a
       header, footer or navigation root o; else inside the article root m; else o. */
    function areaOf(el, main) {
        var e = el;
        var other = false;
        var role;
        var marked = markedArea(el);
        if (marked) {
            return marked;
        }
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

    /* Remembers the focus as the last one of its area and of its zone, which becomes the current zone. */
    function rememberFocus() {
        var a;
        var z;
        if (focusEl && focusEl.isConnected) {
            a = areaOf(focusEl);
            lastByArea[a] = focusEl;
            lastArea = a;
            z = zoneOf(focusEl);
            if (z) {
                lastByZone[z] = focusEl;
                curZone = z;
            }
            zoneBox = null;
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

    /* Whether the document scrolls along axis: its content overflows the viewport that way and the overflow
       applied to the viewport is not hidden. */
    function docScrolls(axis) {
        var cs = window.getComputedStyle(document.documentElement);
        var ov = axis === "x" ? cs.overflowX : cs.overflowY;
        if (document.body && cs.overflowX === "visible" && cs.overflowY === "visible") {
            cs = window.getComputedStyle(document.body);
            ov = axis === "x" ? cs.overflowX : cs.overflowY;
        }
        if (ov === "hidden" || ov === "clip") {
            return false;
        }
        return maxScroll(viewport())[axis] > 0;
    }

    /* Root and the elements inside it matching sel, in document order. */
    function within(root, sel) {
        var list = toArray(root.querySelectorAll(sel));
        if (root !== document && root.matches(sel)) {
            list.unshift(root);
        }
        return list;
    }

    /* The first [data-gnh-pad-scroll] container within the layer, or the document, that scrolls along axis
       and is in area a; else null. */
    function markedScroller(layer, axis, a) {
        var list = within(layer || document, SCROLL_MARK);
        var s;
        var i;
        for (i = 0; i < list.length; i++) {
            s = scrollAxes(list[i]);
            if (s && s[axis] && areaOf(list[i]) === a) {
                return list[i];
            }
        }
        return null;
    }

    /*
     * Scroll container for axis when the document is not used, within the layer if any: the nearest
     * ancestor of the focus marked [data-gnh-pad-scroll] that scrolls that way, else the nearest scrollable
     * ancestor, text fields excluded; else the [data-gnh-pad-scroll] container in the focus's area; else, for
     * the vertical axis, the largest visible scrollable element; null when there is none.
     */
    function innerScroller(axis, layer) {
        var root = layer || document;
        var f = liveFocus();
        var el = f ? parentOf(f) : null;
        var vp = viewport();
        var area = f ? areaOf(f) : lastArea;
        var nearest = null;
        var best = null;
        var bestSize = 0;
        var list;
        var a;
        var r;
        var size;
        var i;
        while (el && el.nodeType === 1) {
            a = isTextField(el) ? null : scrollAxes(el);
            if (a && a[axis]) {
                if (el.matches(SCROLL_MARK)) {
                    return el;
                }
                if (!nearest) {
                    nearest = el;
                }
            }
            if (el === layer) {
                break;
            }
            el = parentOf(el);
        }
        if (nearest) {
            return nearest;
        }
        el = markedScroller(layer, axis, area);
        if (el || axis !== "y") {
            return el;
        }
        list = within(root, "*");
        for (i = 0; i < list.length; i++) {
            el = list[i];
            if (el.scrollHeight <= el.clientHeight) {
                continue;
            }
            a = scrollAxes(el);
            if (!a || !a.y || !isShown(el)) {
                continue;
            }
            r = el.getBoundingClientRect();
            size = el.clientWidth * el.clientHeight;
            if (size > bestSize && r.bottom > 0 && r.top < vp.h && r.right > 0 && r.left < vp.w) {
                best = el;
                bestSize = size;
            }
        }
        return best;
    }

    /* Vertical scroller of page, top, bottom and section: with area a ("m", "s" or "o"), the
       [data-gnh-pad-scroll] container of that area; else null for the document, which is used when it scrolls
       and no layer is open; else an inner scroll container; else the document without a layer, false with one. */
    function pageScroller(a) {
        var layer = activeLayer();
        var sc;
        a = String(a);
        if (VALID_AREAS[a] === true) {
            sc = markedScroller(layer, "y", a);
            if (sc) {
                return sc;
            }
        }
        if (!layer && docScrolls("y")) {
            return null;
        }
        return innerScroller("y", layer) || (layer ? false : null);
    }

    /* innerScroller() for velocity frames, cached for the session and its focus */
    function innerFor(axis, layer) {
        if (!inFrame) {
            return innerScroller(axis, layer);
        }
        if (vel.focus !== focusEl) {
            vel.focus = focusEl;
            vel.inner = {};
        }
        if (vel.inner[axis] === undefined || (vel.inner[axis] && !vel.inner[axis].isConnected)) {
            vel.inner[axis] = innerScroller(axis, layer);
        }
        return vel.inner[axis];
    }

    /* Signals scrolling done here: gnhpadscroll on the scrolled element, on the document for the window. */
    function signalScroll(el) {
        try {
            (el || document).dispatchEvent(new CustomEvent("gnhpadscroll", { bubbles: true }));
        } catch (e) {
            /* the signal is advisory */
        }
    }

    /*
     * Scroll target for one axis: the nearest scroll container at or above the focus that can move
     * that way, else null for the document. Without a focus, the search starts from the element at
     * the viewport center. While a layer is open, or when the document does not scroll, an inner
     * scroll container that can move that way; else null without a layer, false for none with one.
     */
    function scrollTargetFor(axis, d) {
        var layer = activeLayer();
        var el = liveFocus();
        var vp;
        if (!el) {
            vp = viewport();
            el = document.elementFromPoint(vp.w / 2, vp.h / 2);
            if (layer && el && !layer.contains(el)) {
                el = null;
            }
        }
        while (el && el.nodeType === 1) {
            if (canScroll(el, axis, d)) {
                return el;
            }
            if (el === layer) {
                break;
            }
            el = parentOf(el);
        }
        if (!layer && docScrolls(axis)) {
            return null;
        }
        el = innerFor(axis, layer);
        if (el && canScroll(el, axis, d)) {
            return el;
        }
        return layer ? false : null;
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

    /* Smooth scroll of a scroller, null for the document, to x, y; signals scrolling when it moves. */
    function smoothScroll(sc, x, y) {
        var moves = sc ? (Math.abs(sc.scrollLeft - x) >= 1 || Math.abs(sc.scrollTop - y) >= 1) :
            (Math.abs(scrollX() - x) >= 1 || Math.abs(scrollY() - y) >= 1);
        smooth = { el: sc, x: x, y: y, t: now() };
        try {
            (sc || window).scrollTo({ left: x, top: y, behavior: "smooth" });
        } catch (e) {
            if (sc) {
                sc.scrollLeft = x;
                sc.scrollTop = y;
            } else {
                window.scrollTo(x, y);
            }
        }
        if (moves) {
            signalScroll(sc);
        }
    }

    function smoothTo(x, y) {
        smoothScroll(null, x, y);
    }

    /* Vertical scroll position a smooth scroll started here is moving to, else the current one; of the
       scroller sc, the document when omitted. */
    function targetY(sc) {
        var y;
        sc = sc || null;
        y = sc ? sc.scrollTop : scrollY();
        if (smooth && smooth.el === sc && now() - smooth.t < SMOOTH_MS && Math.abs(smooth.y - y) > 1) {
            return smooth.y;
        }
        smooth = null;
        return y;
    }

    /* Scrolls one axis of its scroll target by d CSS px; true if it moved. A moved target is added to moved. */
    function scrollAxis(axis, d, moved) {
        var t = scrollTargetFor(axis, d);
        var before;
        var after;
        if (t === false) {
            return false;
        }
        if (t) {
            before = axis === "x" ? t.scrollLeft : t.scrollTop;
            elementBy(t, axis === "x" ? d : 0, axis === "y" ? d : 0);
            after = axis === "x" ? t.scrollLeft : t.scrollTop;
        } else {
            before = axis === "x" ? scrollX() : scrollY();
            windowBy(axis === "x" ? d : 0, axis === "y" ? d : 0);
            after = axis === "x" ? scrollX() : scrollY();
        }
        if (after === before) {
            return false;
        }
        if (moved.indexOf(t) < 0) {
            moved.push(t);
        }
        return true;
    }

    /* Scrolls by dx, dy and signals each moved target once; true if anything moved. */
    function moveBy(dx, dy) {
        var moved = [];
        if (dx) {
            scrollAxis("x", dx, moved);
        }
        if (dy) {
            scrollAxis("y", dy, moved);
        }
        moved.forEach(function (t) {
            signalScroll(t);
        });
        return moved.length > 0;
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

    /* Instantly scrolls an element into its scroll containers and then into the viewport; a moved container
       gets the scroll signal. */
    function reveal(el) {
        var p = parentOf(el);
        var bx;
        var by;
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
                    bx = p.scrollLeft;
                    by = p.scrollTop;
                    elementBy(p, dx, dy);
                    if (p.scrollLeft !== bx || p.scrollTop !== by) {
                        signalScroll(p);
                    }
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
                inFrame = true;
                withLayerCache(moveBy, [mx, my]);
            }
            placeRing();
        } catch (e) {
            /* keep scrolling until ttl expires */
        } finally {
            inFrame = false;
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

    /* Moves data-gnh-pad-focused to el, or removes it for null. */
    function markFocused(el) {
        var changed = false;
        if (marked && marked !== el) {
            marked.removeAttribute(FOCUSED_ATTR);
            marked = null;
            changed = true;
        }
        if (el && (marked !== el || !el.hasAttribute(FOCUSED_ATTR))) {
            el.setAttribute(FOCUSED_ATTR, "");
            marked = el;
            changed = true;
        }
        if (changed) {
            /* page rules keyed to the attribute may start transitions */
            fadeCache = undefined;
        }
    }

    /* Whether the ring outlines the current zone's box: there is no focus and no layer is open. */
    function zoneBoxLive() {
        return !focusEl && !!zoneBox && zoneBox.isConnected && !activeLayer();
    }

    /* Whether a computed value is present and other than none */
    function notNone(v) {
        return !!v && v !== "none";
    }

    /* Whether a computed style makes its element the containing block of fixed-position descendants */
    function holdsFixed(cs) {
        return notNone(cs.transform) || notNone(cs.perspective) || notNone(cs.filter) || notNone(cs.backdropFilter) ||
            FIXED_WILL_CHANGE.test(cs.willChange || "") || FIXED_CONTAIN.test(cs.contain || "");
    }

    /*
     * The rectangle in which el can show, or null when nothing is left: the viewport narrowed to the padding box of
     * each ancestor that clips it, along each axis whose overflow is not visible. An ancestor clips el only when it
     * is on el's containing block chain, so fixed and absolute elements escape those they are not positioned in, and
     * nothing above the top layer clips. The body and the root element are covered by the viewport.
     */
    function shownArea(el) {
        var vp = viewport();
        var area = { x: 0, y: 0, w: vp.w, h: vp.h };
        var cs = window.getComputedStyle(el);
        var pos = cs.position;
        var e = el;
        var b;
        var x;
        var y;
        while (area && !matchesSafe(e, ":modal", false) && !popoverOpen(e)) {
            e = parentOf(e);
            if (!e || e.nodeType !== 1 || e === document.body || e === document.documentElement) {
                break;
            }
            cs = window.getComputedStyle(e);
            if ((pos === "fixed" && !holdsFixed(cs)) ||
                (pos === "absolute" && cs.position === "static" && !holdsFixed(cs))) {
                continue;
            }
            pos = cs.position;
            if (cs.display === "inline" || cs.display === "contents") {
                continue;
            }
            x = cs.overflowX !== "visible";
            y = cs.overflowY !== "visible";
            if (x || y) {
                b = e.getBoundingClientRect();
                area = intersectRects(area, {
                    x: x ? b.left + e.clientLeft : area.x,
                    y: y ? b.top + e.clientTop : area.y,
                    w: x ? e.clientWidth : area.w,
                    h: y ? e.clientHeight : area.h
                });
            }
        }
        return area;
    }

    /* The parts of an element's fragments that can show, the empty ones left out. */
    function shownFragments(el, frags) {
        var area = shownArea(el);
        var out = [];
        var r;
        var i;
        for (i = 0; area && i < frags.length; i++) {
            r = intersectRects(frags[i], area);
            if (r) {
                out.push(r);
            }
        }
        return out;
    }

    /* The rectangles the ring follows around el: its fragments; for the current zone's box, the box's own rectangle
       if not empty, as what it holds may be scrolled out of it. */
    function ringFragments(el) {
        var r;
        if (el !== zoneBox) {
            return fragments(el);
        }
        r = el.getBoundingClientRect();
        return (r.width > 0 && r.height > 0) ? [toRect(r)] : [];
    }

    /* One fixed div per shown part of a fragment of the focus, else of the current zone's box; hidden without either,
       a fragment, a shown part, or visibility. The outlined element carries data-gnh-pad-focused, shown or not, while
       it has a fragment. A div's style is written only when it changes. */
    function placeRingNow() {
        var f;
        var el = null;
        var frags = [];
        var i;
        var d;
        var s;
        var key;
        try {
            f = liveFocus();
            if (visible && !hidden) {
                el = f || (zoneBoxLive() ? zoneBox : null);
            }
            markFocused(el);
            if (el) {
                frags = ringFragments(el);
            }
            if (!frags.length) {
                markFocused(null);
            } else {
                frags = shownFragments(el, frags);
            }
            for (i = 0; i < MAX_FRAGMENTS; i++) {
                if (i < frags.length) {
                    d = ringDiv(i);
                    key = frags[i].x + "," + frags[i].y + "," + frags[i].w + "," + frags[i].h;
                    if (ringKeys[i] !== key) {
                        s = d.style;
                        s.setProperty("left", frags[i].x + "px", "important");
                        s.setProperty("top", frags[i].y + "px", "important");
                        s.setProperty("width", frags[i].w + "px", "important");
                        s.setProperty("height", frags[i].h + "px", "important");
                        s.setProperty("display", "block", "important");
                        ringKeys[i] = key;
                    }
                } else if (ring[i] && ringKeys[i] !== "none") {
                    ring[i].style.setProperty("display", "none", "important");
                    ringKeys[i] = "none";
                }
            }
        } catch (e) {
            /* the ring is cosmetic */
        }
        try {
            watchMutations(visible && !hidden);
        } catch (e2) {
            /* the ring is still re-placed after calls and on scroll */
        }
    }

    function placeRing() {
        withLayerCache(placeRingNow, []);
    }

    /* Whether a mutation record concerns only the ring's own divs */
    function ringRecord(r) {
        var nodes;
        var i;
        if (isRing(r.target)) {
            return true;
        }
        if (r.type !== "childList") {
            return false;
        }
        nodes = toArray(r.addedNodes).concat(toArray(r.removedNodes));
        for (i = 0; i < nodes.length; i++) {
            if (!isRing(nodes[i])) {
                return false;
            }
        }
        return nodes.length > 0;
    }

    function onMutations(records) {
        var i;
        try {
            for (i = 0; i < records.length; i++) {
                if (!ringRecord(records[i])) {
                    if (!ringRaf) {
                        ringRaf = window.requestAnimationFrame(function () {
                            ringRaf = 0;
                            placeRing();
                        });
                    }
                    return;
                }
            }
        } catch (e) {
            /* observers never throw into the page */
        }
    }

    /* Observes the document while on, so a page that changes after a call re-places the ring. */
    function watchMutations(on) {
        if (on === watching) {
            return;
        }
        if (on) {
            if (typeof window.MutationObserver !== "function") {
                return;
            }
            if (!mutationObserver) {
                mutationObserver = new window.MutationObserver(onMutations);
            }
            mutationObserver.observe(document.documentElement, {
                childList: true, subtree: true, attributes: true, attributeFilter: WATCHED_ATTRS
            });
            watching = true;
        } else {
            mutationObserver.disconnect();
            watching = false;
            if (ringRaf) {
                window.cancelAnimationFrame(ringRaf);
                ringRaf = 0;
            }
        }
    }

    /* Re-places the ring after a click the page may still be reacting to: on the next frame, and after 100 and
       300 ms. */
    function placeAfterClick() {
        window.requestAnimationFrame(function () {
            placeRing();
        });
        window.setTimeout(placeRing, 100);
        window.setTimeout(placeRing, 300);
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
        watchMutations(false);
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

    /* ---------------------------------------------------------------- zones */

    /* The zone of an element: the value of the nearest data-gnh-pad-zone as a positive integer, else 0. */
    function zoneOf(el) {
        var v;
        while (el && el.nodeType === 1) {
            v = el.getAttribute(ZONE_ATTR);
            if (v !== null) {
                v = parseInt(v, 10);
                return v > 0 ? v : 0;
            }
            el = parentOf(el);
        }
        return 0;
    }

    /*
     * Zone mode and the zones that count, in ascending order, as { on, zones, all }: a zone counts when it holds a
     * candidate or a [data-gnh-pad-scroll] container intersecting the viewport, and zone mode is on when no layer
     * is open and at least two zones count. all is the candidates, collected unless passed.
     */
    function zoneInfo(all) {
        var out = { on: false, zones: [], all: all || null };
        var seen = {};
        var vp;
        var list;
        var r;
        var z;
        var i;
        if (!document.querySelector("[" + ZONE_ATTR + "]") || activeLayer()) {
            return out;
        }
        vp = viewport();
        out.all = all || collectAll();
        for (i = 0; i < out.all.length; i++) {
            z = zoneOf(out.all[i].el);
            if (z && !seen[z] && intersects(out.all[i].frags, vp, 0)) {
                seen[z] = true;
                out.zones.push(z);
            }
        }
        list = document.querySelectorAll(SCROLL_MARK);
        for (i = 0; i < list.length; i++) {
            z = zoneOf(list[i]);
            if (!z || seen[z] || !shownBy(list[i], true)) {
                continue;
            }
            r = list[i].getBoundingClientRect();
            if (r.width > 0 && r.height > 0 && intersects([toRect(r)], vp, 0)) {
                seen[z] = true;
                out.zones.push(z);
            }
        }
        out.zones.sort(function (a, b) {
            return a - b;
        });
        out.on = out.zones.length >= 2;
        return out;
    }

    /* The current zone: the focus's zone, else the zone entered last, while it counts; else 0. */
    function currentZone(info) {
        var f = liveFocus();
        var z = f ? zoneOf(f) : 0;
        if (z && info.zones.indexOf(z) >= 0) {
            return z;
        }
        return info.zones.indexOf(curZone) >= 0 ? curZone : 0;
    }

    /* The nearest [data-gnh-pad-scroll] ancestor of an element, else null. */
    function scrollMarkOf(el) {
        var p = parentOf(el);
        while (p && p.nodeType === 1) {
            if (p.matches(SCROLL_MARK)) {
                return p;
            }
            p = parentOf(p);
        }
        return null;
    }

    /* The zone's scroller: its [data-gnh-pad-scroll] container holding el, else its first one, else null. */
    function zoneScroller(z, el) {
        var list = document.querySelectorAll(SCROLL_MARK);
        var first = null;
        var i;
        for (i = 0; i < list.length; i++) {
            if (zoneOf(list[i]) !== z) {
                continue;
            }
            if (el && list[i].contains(el)) {
                return list[i];
            }
            if (!first) {
                first = list[i];
            }
        }
        return first;
    }

    /* The box the ring outlines in a zone without candidates: its scroller, else its first container with a
       fragment in the viewport, else null. */
    function zoneOutline(z) {
        var sc = zoneScroller(z, null);
        var vp = viewport();
        var list;
        var i;
        if (sc) {
            return sc;
        }
        list = document.querySelectorAll("[" + ZONE_ATTR + "]");
        for (i = 0; i < list.length; i++) {
            if (zoneOf(list[i]) === z && hasFragmentIn(list[i], vp)) {
                return list[i];
            }
        }
        return null;
    }

    /* The bounding box of zone z: the union of its shown containers, clipped to the viewport; null without one.
       A container counts with its own box only, so what it holds or scrolls does not widen it. */
    function zoneBounds(z, vp) {
        var list = document.querySelectorAll("[" + ZONE_ATTR + "]");
        var view = { x: 0, y: 0, w: vp.w, h: vp.h };
        var rects = [];
        var r;
        var i;
        for (i = 0; i < list.length; i++) {
            if (zoneOf(list[i]) === z && shownBy(list[i], true)) {
                r = intersectRects(toRect(list[i].getBoundingClientRect()), view);
                if (r) {
                    rects.push(r);
                }
            }
        }
        return unionRects(rects);
    }

    /* Whether box b is stacked under box a: it starts within NEIGHBOR_GAP of a's bottom and ends below it. Boxes that
       overlap more than that, such as a zone and a zone nested in it, are not stacked. */
    function stackedUnder(a, b) {
        var end = a.y + a.h;
        return Math.abs(b.y - end) <= NEIGHBOR_GAP && b.y + b.h > end;
    }

    /* The zone that a Down (d > 0) or Up from zone z hands off to: the nearest in that direction in zone order whose
       box is stacked under or over z's box and overlaps it horizontally; else 0. */
    function neighborZone(info, z, d, vp) {
        var box = zoneBounds(z, vp);
        var i = info.zones.indexOf(z) + d;
        var r;
        for (; box && i >= 0 && i < info.zones.length; i += d) {
            r = zoneBounds(info.zones[i], vp);
            if (r && r.x < box.x + box.w && box.x < r.x + r.w &&
                (d > 0 ? stackedUnder(box, r) : stackedUnder(r, box))) {
                return info.zones[i];
            }
        }
        return 0;
    }

    function rowOf(el) {
        return el.closest(ROW_MARK);
    }

    /* Whether an element is in the secondary part of row */
    function inSecondary(el, row) {
        var s = el.closest(SECONDARY_MARK);
        return !!s && row.contains(s);
    }

    /* Whether an element is, is inside, or holds an element with aria-current other than false */
    function isCurrentEl(el) {
        var a = el.closest("[aria-current]") || el.querySelector("[aria-current]");
        return !!a && a.getAttribute("aria-current") !== "false";
    }

    /* The scroll offsets of an element's [data-gnh-pad-scroll] ancestors, summed as { x, y } */
    function scrollOffset(el) {
        var o = { x: 0, y: 0 };
        var p = scrollMarkOf(el);
        while (p) {
            o.x += p.scrollLeft;
            o.y += p.scrollTop;
            p = scrollMarkOf(p);
        }
        return o;
    }

    function byX(a, b) {
        return a.x - b.x;
    }

    /* Candidates in reading order, placed as if their [data-gnh-pad-scroll] containers were scrolled to the start:
       grouped into lines where their first fragments overlap vertically by more than half the smaller height,
       lines top to bottom, left to right within a line. */
    function readingOrder(list) {
        var keyed = list.map(function (c) {
            var o = scrollOffset(c.el);
            var r = c.frags[0];
            return { c: c, x: r.x + o.x, y: r.y + o.y, h: r.h };
        });
        var lines = [];
        var line = null;
        var out = [];
        var k;
        var i;
        var j;
        keyed.sort(function (a, b) {
            return (a.y - b.y) || (a.x - b.x);
        });
        for (i = 0; i < keyed.length; i++) {
            k = keyed[i];
            if (line && Math.min(line.bottom, k.y + k.h) - k.y > 0.5 * Math.min(k.h, line.bottom - line.top)) {
                line.items.push(k);
                line.bottom = Math.max(line.bottom, k.y + k.h);
            } else {
                line = { top: k.y, bottom: k.y + k.h, items: [k] };
                lines.push(line);
            }
        }
        for (i = 0; i < lines.length; i++) {
            lines[i].items.sort(byX);
            for (j = 0; j < lines[i].items.length; j++) {
                out.push(lines[i].items[j].c);
            }
        }
        return out;
    }

    /* The stops of zone z in reading order: its candidates, a row counting once as its first candidate outside its
       secondary part. */
    function zoneStops(all, z) {
        var out = [];
        var rows = [];
        var row;
        var i;
        for (i = 0; i < all.length; i++) {
            if (zoneOf(all[i].el) !== z) {
                continue;
            }
            row = rowOf(all[i].el);
            if (row) {
                if (inSecondary(all[i].el, row) || rows.indexOf(row) >= 0) {
                    continue;
                }
                rows.push(row);
            }
            out.push(all[i]);
        }
        return readingOrder(out);
    }

    function indexOfEl(list, el) {
        var i;
        for (i = 0; i < list.length; i++) {
            if (list[i].el === el) {
                return i;
            }
        }
        return -1;
    }

    /* The position in stops of an element, or of the stop of its row; -1 for none. */
    function stopIndex(stops, el) {
        var row = rowOf(el);
        var i;
        for (i = 0; i < stops.length; i++) {
            if (stops[i].el === el || (row && rowOf(stops[i].el) === row)) {
                return i;
            }
        }
        return -1;
    }

    /* The candidates of a row in document order as { el, frags }: those of its secondary part, or the others. */
    function rowCandidates(row, secondary) {
        var nodes = row.querySelectorAll(SELECTOR);
        var out = [];
        var outer = null;
        var frags;
        var el;
        var i;
        for (i = 0; i < nodes.length; i++) {
            el = nodes[i];
            if ((outer && outer.contains(el)) || !matchesKind(el) || inSecondary(el, row) !== secondary) {
                continue;
            }
            frags = candidateFragments(el);
            if (frags) {
                outer = el;
                out.push({ el: el, frags: frags });
            }
        }
        return out;
    }

    /* Whether the focus is in a row that has a visible secondary candidate */
    function rowFlag() {
        var f = liveFocus();
        var row = f ? rowOf(f) : null;
        return !!row && rowCandidates(row, true).length > 0;
    }

    /* The elements marked data-gnh-pad-shortcut="key" */
    function shortcutMarks(key) {
        return toArray(document.querySelectorAll("[" + SHORTCUT_ATTR + "]")).filter(function (m) {
            return m.getAttribute(SHORTCUT_ATTR) === key;
        });
    }

    /* The target of key in zone z: of the first mark in that zone that has one, the marked element if it is a
       candidate, else the first candidate inside it; else null. */
    function shortcutOf(z, key) {
        var marks = shortcutMarks(key);
        var nodes;
        var i;
        var j;
        for (i = 0; i < marks.length; i++) {
            if (zoneOf(marks[i]) !== z) {
                continue;
            }
            if (matchesKind(marks[i]) && candidateFragments(marks[i])) {
                return marks[i];
            }
            nodes = marks[i].querySelectorAll(SELECTOR);
            for (j = 0; j < nodes.length; j++) {
                if (matchesKind(nodes[j]) && candidateFragments(nodes[j])) {
                    return nodes[j];
                }
            }
        }
        return null;
    }

    /* Whether the current zone has a y target, on a page with a y mark. Without zone state passed, the current zone is
       taken as the focus's zone, else the zone entered last, so that no candidates are collected. */
    function hasYTarget(info) {
        var f;
        var z;
        if (!shortcutMarks("y").length) {
            return false;
        }
        if (info) {
            z = currentZone(info);
        } else {
            f = liveFocus();
            z = (f ? zoneOf(f) : 0) || curZone;
        }
        return !!z && !!shortcutOf(z, "y");
    }

    /* State flags of snapshots and zone call results: z in zone mode, r for rowFlag(), y in zone mode for
       hasYTarget(). */
    function stateFlags(on, info) {
        return (on ? "z" : "") + (rowFlag() ? "r" : "") + (on && hasYTarget(info) ? "y" : "");
    }

    /* The trailing field of a zone call's result: ~ and the state flags after the call, with the ring placed
       first so that a row it moved to shows its secondary candidates. Zone mode is computed unless on is given. */
    function tail(on) {
        var info;
        placeRing();
        if (on === undefined) {
            info = zoneInfo();
            on = info.on;
        }
        return "~" + stateFlags(on, info);
    }

    /* The viewport span { top, bottom } in which the scroller sc shows content; the viewport for null. */
    function visibleSpan(sc, vp) {
        var b;
        var t;
        if (!sc) {
            return { top: 0, bottom: vp.h };
        }
        b = sc.getBoundingClientRect();
        t = b.top + sc.clientTop;
        return { top: Math.max(0, t), bottom: Math.min(vp.h, t + sc.clientHeight) };
    }

    /* Whether a candidate shows in the viewport within its [data-gnh-pad-scroll] container's view */
    function showsInSpan(c, vp) {
        var span = visibleSpan(scrollMarkOf(c.el), vp);
        var f;
        var i;
        for (i = 0; i < c.frags.length; i++) {
            f = c.frags[i];
            if (f.y + f.h > span.top && f.y < span.bottom && f.x + f.w > 0 && f.x < vp.w) {
                return true;
            }
        }
        return false;
    }

    /* The first stop in reading order that shows; with only set, among those whose [data-gnh-pad-scroll]
       container is sc, null for none. */
    function topVisibleStop(stops, vp, only, sc) {
        var i;
        for (i = 0; i < stops.length; i++) {
            if ((!only || scrollMarkOf(stops[i].el) === sc) && showsInSpan(stops[i], vp)) {
                return stops[i];
            }
        }
        return null;
    }

    /* The last stop in reading order that shows; with only set, among those whose [data-gnh-pad-scroll] container is
       sc, null for none. */
    function bottomVisibleStop(stops, vp, only, sc) {
        var i;
        for (i = stops.length - 1; i >= 0; i--) {
            if ((!only || scrollMarkOf(stops[i].el) === sc) && showsInSpan(stops[i], vp)) {
                return stops[i];
            }
        }
        return null;
    }

    /* The stop of the scroller sc where a move in direction d enters it: the first in reading order not wholly
       above its view for d > 0, the last not wholly below it for d < 0; else null. */
    function entryStop(stops, sc, d, vp) {
        var span = visibleSpan(sc, vp);
        var pick = null;
        var r;
        var i;
        for (i = 0; i < stops.length; i++) {
            if (scrollMarkOf(stops[i].el) !== sc) {
                continue;
            }
            r = unionRects(stops[i].frags);
            if (d > 0 && r.y + r.h > span.top) {
                return stops[i];
            }
            if (d < 0 && r.y < span.bottom) {
                pick = stops[i];
            }
        }
        return pick;
    }

    /* The first stop from position i in direction d that is not in the scroller sc, else null */
    function stopPast(stops, i, d, sc) {
        var j = i + d;
        while (j >= 0 && j < stops.length && scrollMarkOf(stops[j].el) === sc) {
            j += d;
        }
        return (j >= 0 && j < stops.length) ? stops[j] : null;
    }

    /* Scrolls sc instantly by one step of px in direction d, signaling the scroll: D~s and the trailing field. */
    function scrollStep(sc, d, px, on) {
        var before = sc.scrollTop;
        smooth = null;
        elementBy(sc, 0, d * px);
        if (sc.scrollTop !== before) {
            signalScroll(sc);
        }
        return "D~s" + tail(on);
    }

    /* Focuses and reveals a stop: F~<kind> and the trailing field. */
    function focusStop(c, on) {
        focusEl = c.el;
        zoneBox = null;
        reveal(c.el);
        return "F~" + kindOf(c.el) + tail(on);
    }

    /* Moves toward a stop: one step of scroll of its [data-gnh-pad-scroll] container when the stop lies more than
       px outside the container's view and the container can move toward it, else focuses the stop. */
    function approach(c, px, vp, on) {
        var sc = scrollMarkOf(c.el);
        var span = visibleSpan(sc, vp);
        var r = unionRects(c.frags);
        var d = 0;
        var out = 0;
        if (r.y >= span.bottom) {
            d = 1;
            out = r.y - span.bottom;
        } else if (r.y + r.h <= span.top) {
            d = -1;
            out = span.top - (r.y + r.h);
        }
        if (d && out > px && sc && canScroll(sc, "y", d)) {
            return scrollStep(sc, d, px, on);
        }
        return focusStop(c, on);
    }

    /* Makes zone z current with the ring around its outline and no focus: F~z and the trailing field. */
    function outlineZone(z, on) {
        focusEl = null;
        zoneBox = zoneOutline(z);
        return "F~z" + tail(on);
    }

    /* Zone z's last focus while still a candidate, else that focus's row stop, when that shows in its scroller's
       view; else null. */
    function rememberedStop(info, stops, z, vp) {
        var last = lastByZone[z];
        var pick = null;
        var i;
        if (last && last.isConnected && zoneOf(last) === z) {
            i = indexOfEl(info.all, last);
            pick = i >= 0 ? info.all[i] : null;
            if (!pick) {
                i = stopIndex(stops, last);
                pick = i >= 0 ? stops[i] : null;
            }
        }
        return (pick && showsInSpan(pick, vp)) ? pick : null;
    }

    /* Enters zone z: its last focus while still a candidate, else that focus's row stop, when that shows in its
       scroller's view, else the zone's first stop that shows. A zone without a last focus, or with none of its
       stops showing, gets its stop that is, is inside, or holds a data-gnh-pad-initial element; else its
       aria-current stop; else its first stop that shows, else its first stop. A zone without stops becomes current
       with the ring around its outline: F~z. */
    function enterZone(info, z) {
        var vp = viewport();
        var stops = zoneStops(info.all, z);
        var last = lastByZone[z];
        var pick = null;
        var marks;
        var i;
        var j;
        curZone = z;
        pick = rememberedStop(info, stops, z, vp);
        if (!pick && last && last.isConnected && zoneOf(last) === z) {
            pick = topVisibleStop(stops, vp, false, null);
        }
        marks = pick ? [] : toArray(document.querySelectorAll(INITIAL_MARK));
        for (i = 0; i < stops.length && !pick && marks.length; i++) {
            for (j = 0; j < marks.length && !pick; j++) {
                if (marks[j].contains(stops[i].el) || stops[i].el.contains(marks[j])) {
                    pick = stops[i];
                }
            }
        }
        for (i = 0; i < stops.length && !pick; i++) {
            if (isCurrentEl(stops[i].el)) {
                pick = stops[i];
            }
        }
        if (!pick) {
            pick = topVisibleStop(stops, vp, false, null) || stops[0] || null;
        }
        if (pick) {
            return focusStop(pick, info.on);
        }
        return outlineZone(z, info.on);
    }

    /* Enters zone z by a Down (d > 0) or Up handing off to it. Down enters it as zone('n') does. Up prefers the zone's
       scroller: its remembered focus inside the scroller while that shows, else the scroller's bottom visible stop;
       with neither and room to scroll up, the zone's outline, from which further moves scroll or enter the scroller.
       Otherwise its remembered focus while that shows, else its bottom visible stop, else its outline. */
    function enterNeighbor(info, z, d, vp) {
        var stops;
        var sc;
        var pick;
        var inner;
        if (d > 0) {
            return enterZone(info, z);
        }
        stops = zoneStops(info.all, z);
        sc = zoneScroller(z, null);
        pick = rememberedStop(info, stops, z, vp);
        curZone = z;
        if (sc) {
            inner = (pick && sc.contains(pick.el)) ? pick : bottomVisibleStop(stops, vp, true, sc);
            if (inner) {
                return focusStop(inner, info.on);
            }
            if (canScroll(sc, "y", -1)) {
                return outlineZone(z, info.on);
            }
        }
        pick = pick || bottomVisibleStop(stops, vp, false, null);
        return pick ? focusStop(pick, info.on) : outlineZone(z, info.on);
    }

    /* In zone mode, the top visible stop of the current zone, those in its scroller first; else null. */
    function zoneTopVisible(vp) {
        var info = zoneInfo();
        var z = info.on ? currentZone(info) : 0;
        var stops;
        if (!z) {
            return null;
        }
        stops = zoneStops(info.all, z);
        return topVisibleStop(stops, vp, true, zoneScroller(z, focusEl)) || topVisibleStop(stops, vp, false, null);
    }

    /* ---------------------------------------------------------------- calls */

    /* snapshot(reach): S~seq~vw~vh~dpr~sx~sy~maxX~maxY~focus~flags~items, items index,kind,area,x,y,w,h....
       A focus that is disconnected or has no fragment in the viewport is reported as lost (l). Flags: h ring
       hidden, l focus lost, e editing, p initial focus pending, z zone mode, r the focus is in a row with a
       visible secondary candidate, y the current zone has a y target. */
    function snapshot(reach) {
        var vp = viewport();
        var mx = maxScroll(vp);
        var all = collectAll();
        var f = liveFocus();
        var main = mainRoot();
        var parts = [];
        var focusPos = -1;
        var flags = "";
        var info;
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
        info = zoneInfo(all);
        flags += stateFlags(info.on, info);
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

    /* activate(): selects step forward with wrapping; text fields are DOM-focused for editing. After a click or a
       select step the ring is re-placed again shortly, as the page may still be changing. A focus with no shown part
       is not pressed: the focus moves to the current zone's top visible stop in zone mode, else it is revealed,
       and the result is A~n. */
    function activate() {
        var f = liveFocus();
        var frags;
        var tv;
        var k;
        var r;
        if (!f) {
            return "A~n";
        }
        frags = fragments(f);
        if (!shownFragments(f, frags).length) {
            tv = zoneTopVisible(viewport());
            if (tv) {
                focusEl = tv.el;
            } else if (frags.length) {
                reveal(f);
            }
            return "A~n";
        }
        k = kindOf(f);
        if (k === "s") {
            stepSelect(f, 1, true);
            r = "A~s";
        } else if (k === "t" || k === "m") {
            return editField(f);
        } else if (k === "a") {
            r = "A~" + followLink(f);
        } else {
            clickEl(f);
            r = "A~c";
        }
        placeAfterClick();
        return r;
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

    /* After a layer closed, a focus inside it, or none, returns to the focus left outside it when it opened,
       if that is still a candidate. */
    function restoreOrigin(layer) {
        var i = originIndex(layer);
        var o = i >= 0 ? origins[i].el : null;
        invalidateLayer();
        if (i >= 0) {
            origins.splice(i, 1);
        }
        if (o && o.isConnected && (!focusEl || layer.contains(focusEl)) && matchesKind(o) &&
            candidateFragments(o)) {
            focusEl = o;
            if (liveFocus()) {
                reveal(o);
            }
        }
        return "D~1";
    }

    /* Whether a modal dialog may be closed from here: not closedby="none" or data-gnh-pad-noclose. */
    function dialogClosable(d) {
        return (d.getAttribute("closedby") || "").trim().toLowerCase() !== "none" && !d.hasAttribute(NOCLOSE_ATTR);
    }

    /* Asks a dialog to close: requestClose() where available, else a cancelable cancel event and close()
       unless it was canceled. */
    function requestDialogClose(d) {
        if (typeof d.requestClose === "function") {
            d.requestClose();
        } else if (d.dispatchEvent(new Event("cancel", { cancelable: true }))) {
            d.close();
        }
    }

    /* Dispatches an Escape keydown on el; true if a handler canceled it. */
    function sendEscape(el) {
        var ev = new KeyboardEvent("keydown", {
            key: "Escape", code: "Escape", keyCode: 27, which: 27, bubbles: true, cancelable: true
        });
        el.dispatchEvent(ev);
        return ev.defaultPrevented;
    }

    /* The element controlling a listbox: one whose aria-controls or aria-owns names it, else the active element
       when it is outside the listbox and is not the body or root; else null. */
    function listboxController(b) {
        var c = controllersOf(b, false);
        var a;
        if (c.length) {
            return c[0];
        }
        a = deepActive();
        if (a && a !== document.body && a !== document.documentElement && !b.contains(a)) {
            return a;
        }
        return null;
    }

    /* Sends Escape to a listbox and, unless that was canceled and while the listbox still shows, to its
       controller; true if it no longer shows. */
    function escapeListbox(b) {
        var handled = false;
        var c;
        try {
            handled = sendEscape(b);
        } catch (e) {
            /* verified below */
        }
        if (!handled && isShown(b)) {
            c = listboxController(b);
            if (c) {
                try {
                    sendEscape(c);
                } catch (e) {
                    /* verified below */
                }
            }
        }
        return !isShown(b);
    }

    /*
     * closeLayer(): closes the innermost layer, each attempt verified before the next is tried: an open menu
     * popover; a listbox outside popovers, by an Escape keydown on it and on its controller; the topmost modal
     * dialog, unless it opts out; else the nearest open details containing the focus, its own summary included,
     * and focuses that summary. A listbox still shown right after Escape counts as closing, for pages that
     * re-render later; the next call that finds it still shown goes on to the following steps.
     */
    function closeLayer() {
        var ls = layers();
        var f;
        var d;
        var s;
        if (pendingClose && pendingClose !== ls.listbox) {
            pendingClose = null;
        }
        if (ls.popover) {
            try {
                ls.popover.hidePopover();
            } catch (e) {
                /* verified below */
            }
            if (!popoverOpen(ls.popover)) {
                return restoreOrigin(ls.popover);
            }
        }
        if (ls.listbox && pendingClose === ls.listbox) {
            pendingClose = null;
        } else if (ls.listbox) {
            if (escapeListbox(ls.listbox)) {
                return restoreOrigin(ls.listbox);
            }
            pendingClose = ls.listbox;
            invalidateLayer();
            return "D~1";
        }
        if (ls.dialog && dialogClosable(ls.dialog)) {
            try {
                requestDialogClose(ls.dialog);
            } catch (e) {
                /* verified below */
            }
            if (!ls.dialog.open) {
                return restoreOrigin(ls.dialog);
            }
        }
        f = liveFocus();
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
            vel.inner = {};
            vel.focus = focusEl;
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

    /* page(dir, area): smooth page scroll of the page scroller, of area when given, counted from the target of
       a smooth scroll still under way. */
    function page(dir, area) {
        var sc = pageScroller(area);
        var d = num(dir) < 0 ? -1 : 1;
        var vp;
        var mx;
        var y;
        if (sc === null) {
            vp = viewport();
            mx = maxScroll(vp);
            y = clamp(targetY() + d * 0.85 * vp.h, 0, mx.y);
            smoothTo(scrollX(), y);
        } else if (sc) {
            y = clamp(targetY(sc) + d * 0.85 * sc.clientHeight, 0, sc.scrollHeight - sc.clientHeight);
            smoothScroll(sc, sc.scrollLeft, y);
        }
        return "D~1";
    }

    /*
     * Scrolls the page scroller, of area when given, to its top (dir < 0) or bottom and focuses the first or
     * last candidate shown there. Fixed-position candidates are measured where they stand; with an inner
     * scroller, candidates inside it are preferred, else those shown where they stand.
     */
    function scrollToEnd(dir, area) {
        var sc = pageScroller(area);
        var vp = viewport();
        var all = collectAll();
        var pick = null;
        var sx = 0;
        var sy = 0;
        var box = null;
        var ty;
        var b;
        var k;
        var c;
        var el;
        var r;
        if (sc === null) {
            if (dir < 0) {
                sx = scrollX();
                sy = scrollY();
                smoothTo(0, 0);
            } else {
                ty = maxScroll(vp).y;
                sy = scrollY() - ty;
                smoothTo(scrollX(), ty);
            }
        } else if (sc) {
            ty = dir < 0 ? 0 : Math.max(0, sc.scrollHeight - sc.clientHeight);
            sy = sc.scrollTop - ty;
            b = sc.getBoundingClientRect();
            box = { y: b.top + sc.clientTop, h: sc.clientHeight };
            smoothScroll(sc, sc.scrollLeft, ty);
        }
        for (k = 0; k < all.length && !pick; k++) {
            c = all[dir < 0 ? k : all.length - 1 - k];
            el = c.el;
            r = c.frags[0];
            if (box) {
                if (sc.contains(el) && r.y + sy < Math.min(vp.h, box.y + box.h) &&
                    r.y + r.h + sy > Math.max(0, box.y) && r.x < vp.w && r.x + r.w > 0) {
                    pick = el;
                }
            } else if (r.y + sy < vp.h && r.y + r.h + sy > 0 && r.x + sx < vp.w && r.x + r.w + sx > 0) {
                pick = el;
            } else if (r.y < vp.h && r.y + r.h > 0 && isFixed(el)) {
                pick = el;
            }
        }
        if (!pick && box) {
            for (k = 0; k < all.length && !pick; k++) {
                c = all[dir < 0 ? k : all.length - 1 - k];
                if (!sc.contains(c.el) && intersects(c.frags, vp, 0)) {
                    pick = c.el;
                }
            }
        }
        focusEl = pick;
        return pick ? "F~" + kindOf(pick) : "F~n";
    }

    /* top(area): smooth scroll to the top, of the [data-gnh-pad-scroll] scroller of area when given;
       fixed-position candidates are measured where they stand. */
    function top(area) {
        return scrollToEnd(-1, area);
    }

    /* bottom(area): smooth scroll to the bottom; the counterpart of top(). */
    function bottom(area) {
        return scrollToEnd(1, area);
    }

    /* Section starts in document order: the [data-gnh-pad-section] elements in the scroller sc, or in the
       document for the document, when there are any; else h1-h3 in root. */
    function sectionStarts(root, sc) {
        var marked = (sc || document).querySelectorAll("[" + SECTION_ATTR + "]");
        return marked.length ? marked : root.querySelectorAll("h1,h2,h3");
    }

    /*
     * section(dir): smooth-scrolls the next or previous content heading to the top, measured from
     * the target of a smooth scroll still under way. Without such a heading nothing changes and the
     * current focus is reported. Section starts are h1-h3 and [data-gnh-pad-section]; with an inner
     * page scroller, those inside it, measured in its coordinates.
     */
    function section(dir) {
        var sc = pageScroller();
        var root = sc || contentRoot() || document;
        var hs;
        var vp = viewport();
        var boxTop = 0;
        var boxH = vp.h;
        var max;
        var sy;
        var ref;
        var heading = null;
        var headTop = 0;
        var all;
        var pick = null;
        var b;
        var i;
        var r;
        var t;
        var f;
        if (sc === false) {
            f = liveFocus();
            return f ? "F~" + kindOf(f) : "F~n";
        }
        if (sc) {
            b = sc.getBoundingClientRect();
            boxTop = b.top + sc.clientTop;
            boxH = sc.clientHeight;
            max = Math.max(0, sc.scrollHeight - sc.clientHeight);
            sy = sc.scrollTop;
            ref = targetY(sc);
        } else {
            max = maxScroll(vp).y;
            sy = scrollY();
            ref = targetY();
        }
        hs = sectionStarts(root, sc);
        for (i = 0; i < hs.length; i++) {
            r = hs[i].getBoundingClientRect();
            if (!(r.width > 0 || r.height > 0) || inClosedDetails(hs[i])) {
                continue;
            }
            t = r.top - boxTop + sy;
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
        smoothScroll(sc, sc ? sc.scrollLeft : scrollX(), clamp(headTop, 0, max));
        for (i = 0; i < all.length; i++) {
            if (heading.hasAttribute(SECTION_ATTR) &&
                (all[i].el === heading || all[i].el.contains(heading))) {
                pick = all[i].el;
                break;
            }
            if (heading.compareDocumentPosition(all[i].el) & 4) {
                t = all[i].frags[0].y - boxTop + sy;
                if (t >= headTop - 1 && t < headTop + boxH && (!sc || sc.contains(all[i].el))) {
                    pick = all[i].el;
                }
                break;
            }
        }
        focusEl = pick;
        return pick ? "F~" + kindOf(pick) : "F~n";
    }

    /* search(): the first search field that is shown, or is shown once the closed details around
       it are opened; details opened for a field that stays hidden are closed again. While a layer is
       open, only the fields inside it count. */
    function search() {
        var fields = searchFields();
        var layer = activeLayer();
        var i;
        var el;
        var opened;
        var j;
        for (i = 0; i < fields.length; i++) {
            el = fields[i];
            if (layer && !layer.contains(el)) {
                continue;
            }
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

    /* The first candidate that is, is inside, or holds an element marked data-gnh-pad-initial, else null. */
    function markedInitial(all) {
        var marks = toArray(document.querySelectorAll(INITIAL_MARK));
        var i;
        var j;
        for (i = 0; i < all.length && marks.length; i++) {
            for (j = 0; j < marks.length; j++) {
                if (marks[j].contains(all[i].el) || all[i].el.contains(marks[j])) {
                    return all[i];
                }
            }
        }
        return null;
    }

    /* initial(): the first candidate marked data-gnh-pad-initial, pending until the ring shows when it is
       outside the viewport; else the first article candidate in the viewport; else the first one starting
       within the next viewport height below it, pending until the ring shows; else the top visible preferring
       the article. */
    function initial() {
        var vp = viewport();
        var all = collectAll();
        var article = inArea(all, "m");
        var pick = markedInitial(all);
        var i;
        var t;
        initialPending = false;
        if (pick && !intersects(pick.frags, vp, 0)) {
            initialPending = true;
        }
        for (i = 0; i < article.length && !pick; i++) {
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

    /* Controller mode on the root element: data-gnh-pad="active" while visible, and a gnhpadmode event on the
       document when that changes. */
    function signalMode(active) {
        var de = document.documentElement;
        try {
            if (active) {
                de.dataset.gnhPad = "active";
            } else {
                delete de.dataset.gnhPad;
            }
            if (active !== visible) {
                document.dispatchEvent(new CustomEvent("gnhpadmode", { detail: { active: active } }));
            }
        } catch (e) {
            /* the signal is advisory */
        }
    }

    /* setVisible(b): showing clears h, and moves a missing focus, or one with no fragment in the
       viewport, to the top visible stop of the current zone in zone mode, else to the top visible candidate of the
       last focus's area; the box of a current zone without candidates is kept. Hiding sets h and stops watching
       the document. */
    function setVisible(b) {
        var vp;
        var f;
        var tv;
        signalMode(!!(b && b !== "0" && b !== "false"));
        invalidateLayer();
        if (b && b !== "0" && b !== "false") {
            visible = true;
            hidden = false;
            vp = viewport();
            f = liveFocus();
            if (initialPending && f) {
                initialPending = false;
                reveal(f);
            } else if (f ? !hasFragmentIn(f, vp) : !zoneBoxLive()) {
                tv = zoneTopVisible(vp) || topVisibleIn(collectAll(), vp, f ? areaOf(f) : lastArea);
                if (tv) {
                    focusEl = tv.el;
                }
            }
        } else {
            visible = false;
            hidden = true;
            watchMutations(false);
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

    /* focusMarked(name, fallback): focuses and reveals the first candidate that carries data-gnh-pad-key="name", is
       inside an element that does, or contains one; without one, the last candidate in document order matching the
       CSS selector fallback, when given. */
    function focusMarked(name, fallback) {
        var all = collectAll();
        var marks = toArray(document.querySelectorAll("[" + KEY_ATTR + "]"));
        var i;
        var j;
        name = String(name);
        marks = marks.filter(function (m) {
            return m.getAttribute(KEY_ATTR) === name;
        });
        for (i = 0; i < all.length && marks.length; i++) {
            for (j = 0; j < marks.length; j++) {
                if (marks[j].contains(all[i].el) || all[i].el.contains(marks[j])) {
                    focusEl = all[i].el;
                    reveal(focusEl);
                    return "F~" + kindOf(focusEl);
                }
            }
        }
        if (typeof fallback === "string" && fallback) {
            for (i = all.length - 1; i >= 0; i--) {
                if (matchesSafe(all[i].el, fallback, false)) {
                    focusEl = all[i].el;
                    reveal(focusEl);
                    return "F~" + kindOf(focusEl);
                }
            }
        }
        return "F~n";
    }

    /*
     * zone(arg): enters a zone and reveals its focus: arg 'n' the next and 'p' the previous, both without
     * wrapping and the first zone without a current one; 'c' the next with wrapping; 'r' the current one again;
     * a number that zone. Results end with the state flags after the call: F~<kind>~<flags>; F~z~<flags> for a
     * zone without candidates; F~n~<flags> outside zone mode or without such a zone.
     */
    function zone(arg) {
        var info = zoneInfo();
        var zs = info.zones;
        var cur;
        var z = 0;
        var i;
        if (!info.on) {
            return "F~n" + tail(false);
        }
        cur = currentZone(info);
        arg = String(arg);
        if (/^\d+$/.test(arg)) {
            z = zs.indexOf(parseInt(arg, 10)) >= 0 ? parseInt(arg, 10) : 0;
        } else if (arg === "r") {
            z = cur || zs[0];
        } else if (arg === "n" || arg === "c") {
            for (i = 0; i < zs.length && !z; i++) {
                if (zs[i] > cur) {
                    z = zs[i];
                }
            }
            if (!z && arg === "c") {
                z = zs[0];
            }
        } else if (arg === "p") {
            if (!cur) {
                z = zs[0];
            }
            for (i = zs.length - 1; i >= 0 && !z; i--) {
                if (zs[i] < cur) {
                    z = zs[i];
                }
            }
        }
        if (!z) {
            return "F~n" + tail(true);
        }
        return enterZone(info, z);
    }

    /*
     * zoneShortcut(key): in zone mode, focuses and reveals the current zone's target of key 'y', 'lt' or 'rt'. It
     * neither activates the target nor scrolls a zone list. F~<kind>~<flags>; F~n~<flags> without a target, outside
     * zone mode or with a layer open.
     */
    function zoneShortcut(key) {
        var info = zoneInfo();
        var z = info.on ? currentZone(info) : 0;
        var el = z ? shortcutOf(z, String(key)) : null;
        if (!el) {
            return "F~n" + tail(info.on);
        }
        return focusStop({ el: el }, true);
    }

    /*
     * zoneMove(dir, stepPx): moves to the next stop of the current zone in reading order in direction dir, or
     * scrolls one step of stepPx CSS px toward it when it lies more than that outside its scroller's view. A focus
     * that is not a shown stop of the zone moves to the top visible stop instead, of its scroller first. Leaving
     * a scroller that can still move in direction dir scrolls it; entering one goes to its first stop in view, or
     * scrolls it when none is there. Without a next stop, the stop's scroller, else the zone's, scrolls one step
     * when it can; with nothing to scroll, the move enters the zone neighborZone() finds, else a focus that is not
     * a shown stop moves to the zone's top visible stop. Results end with the state flags after the call:
     * F~<kind>~<flags> moved, F~z~<flags> entered a neighbor zone at its outline, D~s~<flags> scrolled, F~x~<flags>
     * up from the first stop of the lowest zone, F~n~<flags> nothing to do or outside zone mode.
     */
    function zoneMove(dir, stepPx) {
        var info = zoneInfo();
        var d = num(dir) < 0 ? -1 : 1;
        var px = Math.max(1, Math.abs(num(stepPx)));
        var vp = viewport();
        var next = null;
        var entry;
        var stops;
        var nsc;
        var sc;
        var nz;
        var z;
        var f;
        var i;
        if (!info.on) {
            return "F~n" + tail(false);
        }
        z = currentZone(info);
        if (!z) {
            return enterZone(info, info.zones[0]);
        }
        stops = zoneStops(info.all, z);
        f = liveFocus();
        i = f ? stopIndex(stops, f) : -1;
        sc = i >= 0 ? scrollMarkOf(stops[i].el) : zoneScroller(z, f);
        if (i >= 0 && !showsInSpan(stops[i], vp)) {
            i = -1;
        }
        if (i < 0) {
            next = topVisibleStop(stops, vp, true, sc);
            if (next) {
                return focusStop(next, true);
            }
            next = entryStop(stops, sc, d, vp);
        } else {
            next = stops[i + d] || null;
            nsc = next ? scrollMarkOf(next.el) : null;
            if (next && nsc !== sc) {
                if (sc && canScroll(sc, "y", d)) {
                    return scrollStep(sc, d, px, true);
                }
                entry = nsc ? entryStop(stops, nsc, d, vp) : next;
                if (!entry && canScroll(nsc, "y", d)) {
                    return scrollStep(nsc, d, px, true);
                }
                next = entry || stopPast(stops, i, d, nsc);
            }
        }
        if (next) {
            return approach(next, px, vp, true);
        }
        if (!sc) {
            sc = zoneScroller(z, null);
        }
        if (sc && canScroll(sc, "y", d)) {
            return scrollStep(sc, d, px, true);
        }
        nz = neighborZone(info, z, d, vp);
        if (nz) {
            return enterNeighbor(info, nz, d, vp);
        }
        if (i < 0) {
            next = topVisibleStop(stops, vp, false, null);
            if (next) {
                return focusStop(next, true);
            }
        }
        return (d < 0 && z === info.zones[0] ? "F~x" : "F~n") + tail(true);
    }

    /* rowActions(): in a row, focuses the next visible candidate of its secondary part in reading order, and after
       the last one the row's stop again: F~<kind>~<flags>; F~n~<flags> outside a row or without one to go to. */
    function rowActions() {
        var f = liveFocus();
        var row = f ? rowOf(f) : null;
        var secs;
        var main;
        var next;
        var i;
        if (!row) {
            return "F~n" + tail();
        }
        /* the page shows the secondary part of the row with the ring */
        placeRing();
        secs = readingOrder(rowCandidates(row, true));
        main = rowCandidates(row, false)[0] || null;
        i = indexOfEl(secs, f);
        next = i >= 0 ? (secs[i + 1] || main) : (secs[0] || null);
        if (!next || next.el === f) {
            return "F~n" + tail();
        }
        return focusStop(next);
    }

    /* refresh(): drops a focus that has left the document, re-applies initial() when the focus is lost, and
       re-places the ring; while the ring shows, a pending initial focus is revealed at once. */
    function refresh() {
        var f;
        if (focusEl && !focusEl.isConnected) {
            focusEl = null;
        }
        f = liveFocus();
        if (!f || !hasFragmentIn(f, viewport())) {
            initial();
            if (visible && !hidden && initialPending && focusEl) {
                initialPending = false;
                reveal(focusEl);
            }
        }
        return "D~1";
    }

    /* Wraps a call: exceptions become E~<message>; the active layer is computed once for the call; afterward
       the focus's area and zone are remembered and the ring re-placed. */
    function guard(fn) {
        return function () {
            try {
                observeLayout();
                return withLayerCache(fn, arguments);
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
        bottom: guard(bottom),
        section: guard(section),
        search: guard(search),
        initial: guard(initial),
        setVisible: guard(setVisible),
        hasSearch: guard(hasSearch),
        area: guard(area),
        hasArea: guard(hasArea),
        focusMarked: guard(focusMarked),
        refresh: guard(refresh),
        zone: guard(zone),
        zoneShortcut: guard(zoneShortcut),
        zoneMove: guard(zoneMove),
        rowActions: guard(rowActions)
    };
})();
