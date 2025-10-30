# test overriding __import__ combined with relative imports


def print_import(name, globals=None, locals=None, fromlist=None, level=0):
    print("import", name, fromlist, level)
    return orig_import(name, globals, locals, fromlist, level)


orig_import = __import__
try:
    __import__("builtins").__import__ = print_import
except AttributeError:
    print("SKIP")
    raise SystemExit

import pkg7.subpkg1.subpkg2.mod3


try:
    # globals must be a dict or None, not a string
    orig_import("builtins", "globals", None, None, 1)
except TypeError:
    print("TypeError")
