import os


class default_cmds:
    @staticmethod
    def help(kernel_instance, *args):
        result = 'Available commands:'
        for cmd, details in kernel_instance.cmd_dict.items():
            result += f'{cmd}: {details['help_text']}'
        return result

    @staticmethod
    def help_debug(kernel_instance, *args):
        result = 'Debug Help: This is a debug version of help command.'
        for cmd, details in kernel_instance.cmd_dict.items():
            result += f'{cmd}: {details['help_text']}\n'
            result += f'{cmd} code: {details['handler']}\n'
        return result

    @staticmethod
    def ls(kernel_instance, *args):
        files = os.listdir(kernel_instance.current_dir)
        return '\n'.join(files)

    @staticmethod
    def cd(kernel_instance, *args):
        if len(args) == 0:
            return 'No directory specified.'
            return
        new_dir = args[0]
        try:
            os.chdir(new_dir)
            kernel_instance.current_dir = os.getcwd()
            return f'Changed directory to {kernel_instance.current_dir}'
        except Exception as e:
            return f'Error changing directory: {e}'


if __name__ == '__main__':
    default_cmds.help(None)  # Test call
