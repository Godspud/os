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
        if len(args) == 0:
            args = (str(kernel_instance.current_dir),)
            try:
                files = os.listdir(args[0])
                return "\n".join(files)
            except PermissionError:
                return f"Permission denied: {args[0]}"
        elif len(args) == 1:
            args = replace_with_home(kernel_instance, args)
            try:
                files = os.listdir(args[0])
                return "\n".join(files)
            except PermissionError:
                return f"Permission denied: {args[0]}"
        else:
            files = []
            for arg in args:
                files.append(f"file's {arg[0]} contents:")
                arg = replace_with_home(kernel_instance, (arg,))
                try:
                    files.append("\n".join(os.listdir(arg[0])))
                except PermissionError:
                    files.append(f"Permission denied: {arg[0]}")
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
        except PermissionError:
            return f"Error changing directory: Permission denied"

    @staticmethod
    def mv(kernel_instance, *args):
        args = replace_with_home(kernel_instance, args)
        try:
            shutil.move(args[0], args[1])
            return f"Moved {args[0]} to {args[1]}"
        except PermissionError:
            return f"Error moving file: Permission denied"

    @staticmethod
    def pwd(kernel_instance, *args):
        return f"Current Directory: {kernel_instance.current_dir}\nHome Directory: {kernel_instance.home_dir}"

    @staticmethod
    def rm(kernel_instance, *args):
        args = replace_with_home(kernel_instance, args)
        try:
            os.remove(args[0])
            return f"Removed file: {args[0]}"
        except IndexError:
            return f"Usage: rm <file_path>"
        except PermissionError:
            return f"Error removing file: Permission denied"

    @staticmethod
    def cp(kernel_instance, *args):
        args = replace_with_home(kernel_instance, args)
        try:
            shutil.copy(args[0], args[1])
            return f"Copied {args[0]} to {args[1]}"
        except IndexError:
            return f"Usage: cp <source> <destination>"
        except PermissionError:
            return f"Error copying file: Permission denied"

    @staticmethod
    def cat(kernel_instance, *args):
        args = replace_with_home(kernel_instance, args)
        output = []
        for arg in args:
            try:
                with open(arg, "r") as f:
                    output.append(f.read())
            except IndexError:
                output.append(f"Usage: cat <file_path>")
            except PermissionError:
                output.append(f"Error reading file: Permission denied")
            except FileNotFoundError:
                output.append(f"File not found: {arg}")
            except IsADirectoryError:
                output.append(f"Is a directory: {arg}")
        return "\n".join(output)

    @staticmethod
    def echo(kernel_instance, *args):
        args = list(args)
        for counter, arg in enumerate(args):
            if "'" in arg:
                arg = arg.replace("'", "")
            if '"' in arg:
                arg = arg.replace('"', "")
            if "'" in arg:
                arg = arg.replace("'", "")
            if '"' in arg:
                arg = arg.replace('"', "")
            args[counter] = arg
        return "".join(args)

    @staticmethod
    def mkdir(kernel_instance, *args):
        args = replace_with_home(kernel_instance, args)
        output = []
        for arg in args:
            try:
                os.mkdir(arg)
                output.append(f"Directory created: {arg}")
            except IndexError:
                output.append(f"Usage: mkdir <directory_name>")
            except PermissionError:
                output.append(f"Error creating directory: Permission denied")
            except FileExistsError:
                output.append(f"Directory already exists: {arg}")
            except FileNotFoundError:
                output.append(f"File not found: {arg}")
        return "\n".join(output)


if __name__ == "__main__":
    import kernel
