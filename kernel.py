from time import perf_counter
from importlib.util import spec_from_file_location, module_from_spec
from os.path import dirname, join, abspath


class kernel:
    def __init__(self, *args):
        start = perf_counter()
        from pathlib import Path

        self.running = True
        self.cmd_dict = {}
        self.current_dir = Path.cwd()
        end = perf_counter()
        print(f"Kernel initialized in {end - start:.6f} seconds")

    def set_running(self, running):
        self.running = running
        if self.running == False:
            print("Shutting down kernel...")

    # register CMDS
    def register_command(self, name, handler, help_text):
        self.cmd_dict[name] = {"handler": handler, "help_text": help_text}
        # format: "file.class.method" or "file.function"
        self.parts = self.cmd_dict[name]["handler"].split(".")
        file_name = parts[0]
        file_path = join(dirname(abspath(__file__)), file_name + ".py")
        spec = spec_from_file_location(file_name, file_path)
        self.module = module_from_spec(spec)
        spec.loader.exec_module(module)

    def execute_command(self, name, *args):
        if name in self.cmd_dict:
            handler = self.cmd_dict[name]["handler"]
            if isinstance(handler, str):

                # Get the function/method
                obj = self.module
                for attr in self.parts[1:]:
                    obj = getattr(obj, attr)
                return obj(self, *args)
            else:
                return handler(self, *args)
        else:
            print("Command not found: " + name)


kernel_instance = kernel()
kernel_instance.register_command("help", "cmds.default_cmds.help", "Help")
kernel_instance.register_command(
    "help_debug", "cmds.default_cmds.help_debug", "Debug Help"
)
kernel_instance.register_command("ls", "cmds.default_cmds.ls", "List files")
while True:
    print(kernel_instance.execute_command(input("Enter command: ")))
