import os
import re

root_dir = r"c:\hmp\GnollHack"
dry_run = False

def find_arguments(s, start_idx):
    idx = start_idx + 1
    depth = 1
    in_string = False
    string_char = None
    escaped = False
    
    args = []
    current_arg = []
    
    while idx < len(s):
        char = s[idx]
        if escaped:
            current_arg.append(char)
            escaped = False
            idx += 1
            continue
            
        if in_string:
            current_arg.append(char)
            if char == '\\':
                escaped = True
            elif char == string_char:
                in_string = False
            idx += 1
            continue
            
        if char in ('"', "'"):
            in_string = True
            string_char = char
            current_arg.append(char)
            idx += 1
            continue
            
        if char == '(':
            depth += 1
        elif char == ')':
            depth -= 1
            if depth == 0:
                args.append("".join(current_arg).strip())
                return args, idx
        
        if char == ',' and depth == 1:
            args.append("".join(current_arg).strip())
            current_arg = []
        else:
            current_arg.append(char)
            
        idx += 1
        
    return None, None

def process_file(path):
    with open(path, "r", encoding="utf-8-sig") as f:
        content = f.read()
        
    modified = False
    new_content = []
    idx = 0
    
    while idx < len(content):
        # Match receiver + DrawImage(
        match = re.search(r"\b((\w+\.)?DrawImage)\s*\(", content[idx:])
        if not match:
            new_content.append(content[idx:])
            break
            
        match_start = idx + match.start()
        method_name = match.group(1)
        
        # Check if the line is commented out
        line_start = content.rfind("\n", 0, match_start) + 1
        prefix = content[line_start:match_start]
        if "//" in prefix:
            open_paren_idx = idx + match.end() - 1
            args, end_paren_idx = find_arguments(content, open_paren_idx)
            next_idx = (end_paren_idx + 1) if (end_paren_idx is not None) else (open_paren_idx + 1)
            new_content.append(content[idx:next_idx])
            idx = next_idx
            continue
            
        indent = prefix
        if not indent.isspace():
            indent = " " * (match_start - line_start)
            
        new_content.append(content[idx:match_start])
        
        open_paren_idx = idx + match.end() - 1
        args, end_paren_idx = find_arguments(content, open_paren_idx)
        
        if args is None:
            new_content.append(content[match_start:open_paren_idx+1])
            idx = open_paren_idx + 1
            continue
            
        stmt_end_idx = end_paren_idx + 1
        while stmt_end_idx < len(content) and content[stmt_end_idx].isspace():
            stmt_end_idx += 1
        if stmt_end_idx < len(content) and content[stmt_end_idx] == ';':
            stmt_end = ";"
            stmt_end_idx += 1
        else:
            stmt_end = ""
            stmt_end_idx = end_paren_idx + 1
            
        has_sampling = any("SKSamplingOptions" in arg for arg in args)
        
        if has_sampling:
            new_content.append(content[match_start:stmt_end_idx])
        else:
            last_arg = args[-1].lower() if args else ""
            is_paint = "paint" in last_arg
            
            if is_paint:
                paint_arg = args[-1]
                args_except_paint = args[:-1]
                args_str_except_paint = ", ".join(args_except_paint)
                
                cond_block = (
                    f"{method_name}({args_str_except_paint}\n"
                    f"#if GNH_MAUI\n"
                    f"{indent}, SKSamplingOptions.Default\n"
                    f"#endif\n"
                    f"{indent}, {paint_arg}){stmt_end}"
                )
            else:
                args_str_xamarin = ", ".join(args)
                
                cond_block = (
                    f"{method_name}({args_str_xamarin}\n"
                    f"#if GNH_MAUI\n"
                    f"{indent}, SKSamplingOptions.Default\n"
                    f"#endif\n"
                    f"{indent}){stmt_end}"
                )
            
            new_content.append(cond_block)
            modified = True
            
        idx = stmt_end_idx
        
    if modified and not dry_run:
        with open(path, "w", encoding="utf-8-sig") as f:
            f.write("".join(new_content))
        print(f"UPDATED conditionally: {path}")

target_files = [
    r"win\win32\xpl\GnollHackX\GnollHackX\GHApp.cs",
    r"win\win32\xpl\GnollHackX\GnollHackX\BackgroundView.cs",
    r"win\win32\xpl\GnollHackX\GnollHackX\ImageCarousel.cs",
    r"win\win32\xpl\GnollHackX\GnollHackX\Pages\Game\GamePage.xaml.cs"
]

for rel_path in target_files:
    abs_path = os.path.join(root_dir, rel_path)
    if os.path.exists(abs_path):
        process_file(abs_path)
