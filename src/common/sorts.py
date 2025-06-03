import sys
import re

def parse_functions(lines):
    functions = []
    current_name = None
    current_body = []
    empty_lines = 0

    for line in lines:
        if re.match(r'^<.*>:$', line.strip()):
            if current_name is not None:
                functions.append((current_name, current_body))
            current_name = line
            current_body = []
            empty_lines = 0
        elif line.strip() == '':
            empty_lines += 1
            current_body.append(line)
            if empty_lines == 2 and current_name is not None:
                functions.append((current_name, current_body))
                current_name = None
                current_body = []
                empty_lines = 0
        else:
            empty_lines = 0
            current_body.append(line)

    if current_name is not None:
        functions.append((current_name, current_body))

    return functions

def main():
    lines = sys.stdin.readlines()
    functions = parse_functions(lines)
    functions.sort(key=lambda fn: fn[0].strip()[1:-2])  # extract name between < and >:

    for name, body in functions:
        sys.stdout.write(name)
        sys.stdout.writelines(body)
        sys.stdout.write('\n')  # ensure spacing between functions

if __name__ == "__main__":
    main()
