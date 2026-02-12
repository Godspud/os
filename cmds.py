import os
import shutil


def replace_with_home(kernel_instance, args):
    listt = []
    for object in args:
        listt.append(object)
    for object in listt:
        if "~" in object:
            temp = object.replace("~", str(kernel_instance.home_dir))
            listt[listt.index(object)] = temp
    args = tuple(listt)
    return args


class default_cmds:
    @staticmethod
    def help(kernel_instance, *args):
        result = "Available commands:"
        for cmd, details in kernel_instance.cmd_dict.items():
            result += f"{cmd}: {details['help_text']}\n"
        return result

    @staticmethod
    def help_debug(kernel_instance, *args):
        result = "Debug Help: This is a debug version of help command."
        for cmd, details in kernel_instance.cmd_dict.items():
            result += f"{cmd}: {details['help_text']}\n"
            result += f"{cmd} code: {details['handler']}\n"
        return result

    @staticmethod
    def ls(kernel_instance, *args):
        print(kernel_instance.current_dir)
        if len(args) == 0:
            args = (str(kernel_instance.current_dir),)
        else:
            print(args)
            args = replace_with_home(kernel_instance, args)
            print(args)
        files = os.listdir(args[0])
        return "\n".join(files)

    @staticmethod
    def cd(kernel_instance, *args):
        args = replace_with_home(kernel_instance, args)
        if len(args) == 0:
            return "No directory specified."
        new_dir = args[0]
        try:
            os.chdir(new_dir)
            kernel_instance.current_dir = os.getcwd()
            return f"Changed directory to {kernel_instance.current_dir}"
        except Exception as e:
            return f"Error changing directory: {e}"

    @staticmethod
    def mv(kernel_instance, *args):
        args = replace_with_home(kernel_instance, args)
        try:
            shutil.move(args[0], args[1])
            return f"Moved {args[0]} to {args[1]}"
        except Exception as e:
            return f"Error moving file: {e}"

    @staticmethod
    def info(kernel_instance, *args):
        return f"Current Directory: {kernel_instance.current_dir}\nHome Directory: {kernel_instance.home_dir}"


if __name__ == "__main__":
    default_cmds.help(None)  # Test call
