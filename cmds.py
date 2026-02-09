import os


class default_cmds:
    @staticmethod
    def help(kernel_instance, *args):
        print("Available commands:")
        for cmd, details in kernel_instance.cmd_dict.items():
            print(f"{cmd}: {details['help_text']}")

    @staticmethod
    def help_debug(kernel_instance, *args):
        print("Debug Help: This is a debug version of help command.")
        for cmd, details in kernel_instance.cmd_dict.items():
            print(f"{cmd}: {details['help_text']}")
            print(f"{cmd} code: {details['handler']}")

    @staticmethod
    def ls(kernel_instance, *args):
        print("Listing files... ")

        files = os.listdir(kernel_instance.current_dir)
        print("\n".join(files))

    @staticmethod
    def cd(kernel_instance, *args):
        if len(args) == 0:
            print("No directory specified.")
            return
        new_dir = args[0]
        try:
            os.chdir(new_dir)
            kernel_instance.current_dir = os.getcwd()
            print(f"Changed directory to {kernel_instance.current_dir}")
        except Exception as e:
            print(f"Error changing directory: {e}")


if __name__ == "__main__":
    default_cmds.help(None)  # Test call
