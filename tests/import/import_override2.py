# test overriding __import__ combined with importing from the filesystem


def custom_import(name, globals, locals, fromlist, level):
    print("import", name, fromlist, level)
    return orig_import(name, globals, locals, fromlist, level)


orig_import = __import__
try:
    __import__("builtins").__import__ = custom_import
except AttributeError:
    print("SKIP")
    raise SystemExit

# import calls __import__ behind the scenes
import pkg7.subpkg1.subpkg2.mod3


try:
    # globals must be a dict or None, not a string
    orig_import("builtins", "globals", None, None, 0)
except TypeError:
    print("TypeError")
try:
    # ... same for relative imports (level > 0)
    orig_import("builtins", "globals", None, None, 1)
except TypeError:
    print("TypeError")
