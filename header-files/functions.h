typedef struct struct_arguments struct_arguments;

struct struct_arguments{
	int id;
	char *common_dir;
	char *input_dir;
	char *mirror_dir;
	int buffer_size;
	char *log_file;
};

int takeArguments(struct_arguments **arguments,int argc,char **argv);
int checkAndCreateDirs(struct_arguments *arguments);
int createFileInCommon(struct_arguments *arguments);
int createLogFile(struct_arguments *arguments);
int countDigits(int x);


int isDirectory(const char *path);
void cleanDirOrFile(char *directory_or_file);
