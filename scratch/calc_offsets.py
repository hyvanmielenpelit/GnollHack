class Layout:
    def __init__(self):
        self.offset = 0

    def add(self, size, align, name):
        # align offset
        rem = self.offset % align
        if rem != 0:
            self.offset += align - rem
        print(f"{name:<20}: {self.offset}")
        self.offset += size

l = Layout()
l.add(8, 8, "nobj")
l.add(8, 8, "v")
l.add(8, 8, "cobj")
l.add(4, 4, "o_id")
l.add(4, 4, "owt")
l.add(2, 2, "otyp")
l.add(1, 1, "ox")
l.add(1, 1, "oy")
l.add(1, 1, "ox0")
l.add(1, 1, "oy0")
l.add(8, 8, "quan")
l.add(8, 8, "age")
l.add(8, 8, "owornmask")
l.add(8, 8, "item_flags")
l.add(8, 8, "speflags")
l.add(2, 2, "enchantment")
l.add(2, 2, "charges")
l.add(2, 2, "special_quality")
l.add(2, 2, "oartifact")
l.add(1, 1, "oclass")
l.add(1, 1, "invlet")
l.add(1, 1, "mythic_prefix")
l.add(1, 1, "mythic_suffix")
l.add(1, 1, "exceptionality")
l.add(1, 1, "elemental_enchantment")
l.add(1, 1, "material")
l.add(1, 1, "recharged")
l.add(1, 1, "where")
l.add(1, 1, "timed")
l.add(4, 4, "corpsenm")
l.add(4, 4, "usecount")
l.add(4, 4, "oeaten")
l.add(8, 8, "bitflags")
l.add(1, 1, "oeroded")
l.add(1, 1, "oeroded2")
l.add(4, 4, "firing_m_id")
l.add(4, 4, "o_id_memory")
l.add(4, 4, "m_id_memory")
l.add(2, 2, "cooldownleft")
l.add(2, 2, "repowerleft")
l.add(2, 2, "detectioncount")
l.add(2, 2, "invokeleft")
l.add(1, 1, "invokeon")
l.add(1, 1, "special_tileset")
l.add(4, 4, "glyph")
l.add(4, 4, "gui_glyph")
l.add(8, 8, "oextra")

# struct align is max align
rem = l.offset % 8
if rem != 0:
    l.offset += 8 - rem
print(f"Total size: {l.offset}")
