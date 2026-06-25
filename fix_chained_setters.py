import os, re

files = [
    'src/do.c', 'src/do_name.c', 'src/eat.c', 'src/encounter.c',
    'src/end.c', 'src/fountain.c', 'src/invent.c', 'src/makemon.c',
    'src/mkobj.c', 'src/mon.c', 'src/muse.c', 'src/objnam.c',
    'src/polyself.c', 'src/potion.c', 'src/pray.c', 'src/read.c',
    'src/sounds.c', 'src/spell.c', 'src/trap.c', 'src/uhitm.c',
    'src/u_init.c', 'src/zap.c'
]

# We need a function to parse lines like:
# set_obj_known(otmp, set_obj_dknown(otmp, otmp->bknown = otmp->rknown = otmp->nknown = 1));
# and convert them to comma-separated setters:
# set_obj_known(otmp, 1), set_obj_dknown(otmp, 1), set_obj_bknown(otmp, 1), set_obj_rknown(otmp, 1), set_obj_nknown(otmp, 1);

def fix_line(line):
    orig = line
    # Match patterns like: set_obj_A(obj, set_obj_B(obj, X))
    # and obj->C = obj->D = X
    
    # Simple unrolling regex for nested sets:
    # set_obj_X(VAR, Y) -> we need to extract X, VAR, and Y.
    # But Y might be another set_obj_Z(VAR, W) or VAR->W = V.
    
    # It's easier to find the variable and all field names.
    # Let's extract all field names.
    fields = []
    
    # Find all set_obj_X and is_obj_X
    m_macros = re.findall(r'(?:set|is)_obj_([a-zA-Z0-9_]+)\([^,]+,?\s*', line)
    fields.extend(m_macros)
    
    # Find all ->X =
    m_arrows = re.findall(r'->([a-zA-Z0-9_]+)\s*=', line)
    fields.extend(m_arrows)
    
    if not fields: return line
    
    # Find variable
    m_var = re.search(r'set_obj_[a-zA-Z0-9_]+\(\s*([^,]+)\s*,', line)
    if not m_var: return line
    var = m_var.group(1).strip()
    
    # Find value (the last number or boolean)
    m_val = re.search(r'=\s*([^;,\)\s]+)[\s\)]*;?', line)
    val = "1"
    if m_val:
        val = m_val.group(1)
    else:
        # If no =, maybe the inner most is just a value like 0 or 1
        m_val2 = re.search(r',\s*([A-Z0-9_]+)\s*\)+;', line)
        if m_val2:
            val = m_val2.group(1)
        else:
            # Let's check for things like is_obj_dknown(obj)
            if 'is_obj_' in line and 'set_obj_' not in line:
                return line
            m_val3 = re.search(r',\s*([0-9a-zA-Z_]+)\s*\)+', line)
            if m_val3:
                val = m_val3.group(1)

    # Reconstruct the line!
    # Wait, what if it's "set_obj_known(obj, set_obj_bknown(obj, is_obj_dknown(obj)))"?
    # Then value is `is_obj_dknown(obj)`
    if 'is_obj_' in line:
        # manual fix for end.c
        pass
        
    setters = [f'set_obj_{f}({var}, {val})' for f in fields]
    
    # preserve indentation
    indent = line[:len(line) - len(line.lstrip())]
    
    if line.strip().endswith(';'):
        return indent + ', '.join(setters) + ';\n'
    else:
        return indent + ', '.join(setters) + '\n'

def process_file(fname):
    filepath = os.path.join('c:/hmp/GnollHack', fname)
    if not os.path.exists(filepath): return
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # We will do regex sub for the specific patterns.
    # Pattern 1: set_obj_A(var, set_obj_B(var, X));
    def replacer(m):
        full = m.group(0)
        # Extract var
        var_m = re.search(r'\(\s*([^,]+)\s*,', full)
        if not var_m: return full
        var = var_m.group(1)
        
        # Extract fields
        fields = re.findall(r'(?:set_obj_|->)([a-zA-Z0-9_]+)(?:\(|(?:\s*=))', full)
        
        # Extract value
        val_m = re.search(r'(?:=\s*|,\s*)([^=,]+?)\)+$', full)
        if not val_m: return full
        val = val_m.group(1).strip()
        
        setters = [f'set_obj_{f}({var}, {val})' for f in fields]
        return ', '.join(setters)

    # Regex to match the whole chained expression ending with )
    # It starts with set_obj_, and ends with ))+
    new_content = re.sub(r'set_obj_[a-zA-Z0-9_]+\([^;]+?\)+', lambda m: replacer(m) if 'set_obj_' in m.group(0)[8:] or '->' in m.group(0) else m.group(0), content)
    
    if new_content != content:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print(f'Fixed {fname}')

for fname in files:
    if fname != 'src/end.c':
        process_file(fname)
