import sys, os, re

fields = [
    'cursed', 'blessed', 'unpaid', 'no_charge', 'known', 'dknown', 'bknown', 'rknown',
    'oerodeproof', 'olocked', 'obroken', 'otrapped', 'lamplit', 'makingsound', 'globby',
    'greased', 'nomerge', 'was_thrown', 'has_special_tileset', 'in_use', 'bypass',
    'cknown', 'lknown', 'tknown', 'nknown', 'aknown', 'mknown', 'rotknown',
    'degraded_horn', 'opoisoned'
]
fields_pattern = '(?:' + '|'.join(fields) + ')'

PTR = r'((?:\(\*?[a-zA-Z_]\w*\))|(?:[a-zA-Z_][\w\[\]\.]*))'

p_assign = re.compile(PTR + r'\s*->\s*(' + fields_pattern + r')\s*=\s*([^;,\n]+)')
p_assign_dot = re.compile(PTR + r'\s*\.\s*(' + fields_pattern + r')\s*=\s*([^;,\n]+)')

p_eq_true1 = re.compile(PTR + r'\s*->\s*(' + fields_pattern + r')\s*==\s*TRUE\b')
p_eq_true2 = re.compile(PTR + r'\s*->\s*(' + fields_pattern + r')\s*==\s*1\b')
p_eq_false1 = re.compile(PTR + r'\s*->\s*(' + fields_pattern + r')\s*==\s*FALSE\b')
p_eq_false2 = re.compile(PTR + r'\s*->\s*(' + fields_pattern + r')\s*==\s*0\b')
p_neq_false1 = re.compile(PTR + r'\s*->\s*(' + fields_pattern + r')\s*!=\s*FALSE\b')
p_neq_false2 = re.compile(PTR + r'\s*->\s*(' + fields_pattern + r')\s*!=\s*0\b')

p_eq_true_dot1 = re.compile(PTR + r'\s*\.\s*(' + fields_pattern + r')\s*==\s*TRUE\b')
p_eq_true_dot2 = re.compile(PTR + r'\s*\.\s*(' + fields_pattern + r')\s*==\s*1\b')
p_eq_false_dot1 = re.compile(PTR + r'\s*\.\s*(' + fields_pattern + r')\s*==\s*FALSE\b')
p_eq_false_dot2 = re.compile(PTR + r'\s*\.\s*(' + fields_pattern + r')\s*==\s*0\b')
p_neq_false_dot1 = re.compile(PTR + r'\s*\.\s*(' + fields_pattern + r')\s*!=\s*FALSE\b')
p_neq_false_dot2 = re.compile(PTR + r'\s*\.\s*(' + fields_pattern + r')\s*!=\s*0\b')

p_access = re.compile(PTR + r'\s*->\s*(' + fields_pattern + r')\b(?!\s*=[^=])')
p_access_dot = re.compile(PTR + r'\s*\.\s*(' + fields_pattern + r')\b(?!\s*=[^=])')

def r_assign(m): return f'set_obj_{m.group(2)}({m.group(1)}, {m.group(3)})'
def r_assign_dot(m): return f'set_obj_{m.group(2)}(&({m.group(1)}), {m.group(3)})'

def r_eq_true(m): return f'is_obj_{m.group(2)}({m.group(1)})'
def r_eq_false(m): return f'!is_obj_{m.group(2)}({m.group(1)})'
def r_eq_true_dot(m): return f'is_obj_{m.group(2)}(&({m.group(1)}))'
def r_eq_false_dot(m): return f'!is_obj_{m.group(2)}(&({m.group(1)}))'

def r_access(m): return f'is_obj_{m.group(2)}({m.group(1)})'
def r_access_dot(m): return f'is_obj_{m.group(2)}(&({m.group(1)}))'

dirs = ['c:/hmp/GnollHack/src', 'c:/hmp/GnollHack/include', 'c:/hmp/GnollHack/win', 'c:/hmp/GnollHack/sys', 'c:/hmp/GnollHack/util']

for d in dirs:
    for root, _, files in os.walk(d):
        for file in files:
            if file.endswith(".c") or file.endswith(".h") or file.endswith(".y") or file.endswith(".l"):
                filepath = os.path.join(root, file)
                try:
                    with open(filepath, 'r', encoding='utf-8') as f:
                        content = f.read()
                except UnicodeDecodeError:
                    continue
                
                if not re.search(r'(?:->|\.)(?:' + '|'.join(fields) + r')\b', content):
                    continue
                
                orig_content = content

                content = p_assign.sub(r_assign, content)
                content = p_assign_dot.sub(r_assign_dot, content)

                content = p_eq_true1.sub(r_eq_true, content)
                content = p_eq_true2.sub(r_eq_true, content)
                content = p_eq_false1.sub(r_eq_false, content)
                content = p_eq_false2.sub(r_eq_false, content)
                content = p_neq_false1.sub(r_eq_true, content)
                content = p_neq_false2.sub(r_eq_true, content)

                content = p_eq_true_dot1.sub(r_eq_true_dot, content)
                content = p_eq_true_dot2.sub(r_eq_true_dot, content)
                content = p_eq_false_dot1.sub(r_eq_false_dot, content)
                content = p_eq_false_dot2.sub(r_eq_false_dot, content)
                content = p_neq_false_dot1.sub(r_eq_true_dot, content)
                content = p_neq_false_dot2.sub(r_eq_true_dot, content)

                content = p_access.sub(r_access, content)
                content = p_access_dot.sub(r_access_dot, content)

                if content != orig_content:
                    with open(filepath, 'w', encoding='utf-8') as f:
                        f.write(content)
                    print(f'Converted {filepath}')
