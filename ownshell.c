/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------
Program/project name:

Aurthors: 
Xujie Zheng(xj721386) A00394753
Fengxiao Yuan(FengxiaYuan) A00394754
Jie Zhang(JieZhang0918) A00331569

Description: building a custom shell that include the following features
1.check if the command is found
2.listing the command that has been input command cmdhistory (this will print the cmd entered even it is an unsuccessful input, give a chance for the user for checking)
3.use "usecmdhistory" and 0~20 can access use the cmd that is type directly
4.ctrl+c or ctrl+z to end(quit) the shell
5.after modify the config.h file, another compilation is require also another ./a.out run
6.
7.
8.
9.
10.
-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>

/*******all the function should be declare here with the short explanation of what this function do and how(by calling what and what should be the argument)************************/
int parse(char* input, char*** arguments,int* ampersandflag, int* para);//parse the line of input, seperate them into arguments array, the return number is how many argument there is, and arguments[0] is command
//if pipelineflag is set, the******************************8
//if it detectes the parentesess the order of cmd execution will change accordingly before return from function and para will return the number of arguments for each cmd entered
//follow two function can be combined together
char* check_cmd(char* commands,char* dir, int depth, int further);//function that check the command if it is in the directory
void print(int(*p)[62]);
void star(int (*p)[62],int i,int j,int n);




//main function
void main(int argc, char *argv[]){
	char** arg_his= (char **) malloc(MAX_LOG*sizeof(char*));//store input from user
	int j,k;
	for(j=0;j<MAX_LOG;j++) arg_his[j] = (char*) malloc(MAX_LENGTH+1);//the +1 is for null
	lognumber=0;//initialize the global variable for the arguments history

	//return value from check_cmd
	char* tab_log = (char*) malloc(MAX_LENGTH+1);//the +1 is for null and it can only show 20 similiar tab result

	if(argc>1){
		fprintf(stderr, "this program %s does not take argument\n",argv[0]);
		exit(error);//return error/1 indicate taht this is a abnormal exit

	}

	int a[20][62]={0};
	star(a,2,25,3);
	print(a);
	fprintf(stdout,"================--^-^----WELCOME----^-^--================\n");
	fprintf(stdout,"                @  +  @            @ +  @                \n");
	fprintf(stdout,"                   _                 _                   \n");
	fprintf(stdout,"Group name: NoName\nGroup menber:Xujie Zheng(xj721386) A00394753\nFengxiao Yuan(FengxiaYuan) A00394754\nJie Zhang(JieZhang0918) A00331569\n");
 	fprintf(stdout,"===================LET US START THE PARTY================\n");
	//start of the main function
	while(1){//constantly running shell

		//initialize the global variable
		
			
		//varriable declaration
		char* input = (char *) malloc(MAX_LENGTH);
		char*** arguments= (char ***) malloc(MAX_CMDSTRING*sizeof(char**));
       		for(j=0;j<MAX_CMDSTRING;j++){
			arguments[j] = (char**) malloc(MAX_ARGS*sizeof(char*));
			for(k=0;k<MAX_ARGS;k++)
				arguments[j][k] = (char*) malloc(MAX_LENGTH+1);//the +1 is for null
		}
		int* ampersandflag = malloc(MAX_ARGS*sizeof(int));
		int v;
		for(v=0;v<=MAX_ARGS;v++) ampersandflag[v] = -1;
		int* para = (int*) malloc(MAX_CMDSTRING*sizeof(int));//if there is somethin like ls -lt| head, then the parameters should be parameters[0]=2, parameters[1]=1 etc.
		int cargc; //same example as above, cargc = 2, indicate there is totally 2 cmd is stored in arguments

/****************************end of decleartion and start of program************************************************************************************************************/
		fprintf(stdout,"$ ");//shell command line symbol

		//cannot handle the tab or up arrow input in concurren. Maybe create a new process to monitor the input from user might work, but it will take a lot of work
		if(!fgets(input, MAX_LENGTH, stdin)) break;//if the argument is sending longer than MAX_LENGTH the program will quit


		strcpy(arg_his[lognumber],input);
		lognumber++;
		
	

		
		//parse the input
		cargc = parse(input, arguments,ampersandflag,para);
		arguments[0][para[0]+1] = NULL;//let the last argument be null


	
		if(arguments[0][para[0]][strlen(arguments[0][para[0]])-1] == '\t'){//need to auto finish
			//get current directory and store it so that later on it can be retreat
			char* cwd = getcwd(NULL,0);
			if(cwd==NULL) fprintf(stderr,"getcwd() error");
#if debugdir
			else fprintf(stdout,"current directory is %s\n",cwd);
#endif

			//fprintf(stdout,"getcha->%c<-\n\n",arguments[0][cargc][strlen(arguments[0][cargc])-1]);
			//initialize an array for store the value from check_cmd for tab feature

			tab_log = NULL;
			//further is equal to 0
			tab_log = check_cmd(arguments[0][para[0]],cwd, 0,0);
			
			if(tab_log != NULL){
				//this will make the program run 2 times for reasons that is unkonwn
				
				/*fprintf(stdout,"do you mean %s? y/n\n",tab_log);
				if(getchar() == 'y') */
					arguments[0][para[0]] = tab_log;

			}
			else fprintf(stderr,"more than one file is matched or no such file in this folder\n");
			tab_log = NULL;	
			chdir(cwd);//go back to current cwd
			free(cwd);
		}	
			
			
		//special case command
		if(strcmp(arguments[0][0],"cmdhistory")==0){//print the cmd history
			for(j = 0; j<lognumber;j++) fprintf(stdout,"%d %s",j,arg_his[j]);	
			tab_log = "cmdhistory";
		}
		if(strcmp(arguments[0][0],"usecmdhistory")==0){//use one of the command from history
			//parse the input of the desired input from the history
			int targetlog = atoi(arguments[0][1]);
			cargc = parse(arg_his[targetlog], arguments,ampersandflag,para);//change the second argument to interger so that the arg_his can be access
			arguments[0][para[0]+1] = NULL;//let the last argument be null
			tab_log = "usecmdhistory";		
		}
		if(strcmp(arguments[0][0],"cd")==0){
			chdir(arguments[0][1]);			
			tab_log = "cd";
		}
		if(strcmp(arguments[0][0],"exit")==0){
			exit(0);
		}
	

		
/*************************going to create a new process to run************************************************************************************************************************/		
        pid_t pid;
        int n;
        pid_t* child_pids= malloc((cargc+1)*sizeof(pid_t));

        for (n=0; n <= cargc; n++){
				if(ampersandflag[n] == 0){
			
					siginfo_t *infop;
					if(n>0){
                        		int status = 0;
#if debugfork
                        		fprintf(stdout,"CHILD: waiting for child %d (%d)\n", (n-1), child_pids[n-1]);
#endif
                       			int rc_pid = waitpid(child_pids[n-1], &status, 0);
#if debugfork
                       			fprintf(stdout,"CHILD: Child: %d returned value is: %d\n", (n-1), WEXITSTATUS(status));
#endif
						
					}
				}

                /* Multiple child forking */
                switch(pid = fork()){
                        case -1:
                                /* something went wrong */
                                /* parent exits */
                                perror("fork");
                                exit(n);

                        case 0:
                                /*Children process*/
                                pid = getpid();
				

/**************************************************************childproc******************************************************************************************************************/
				char* cwd = getcwd(NULL,0);
				if(cwd==NULL) fprintf(stderr,"getcwd() error");
#if debugdir
				else fprintf(stdout,"current directory is %s\n",cwd);
#endif
			
				tab_log= NULL;
				tab_log = check_cmd(arguments[n][0],"/bin",0,1);
				if(tab_log==NULL) {
					tab_log=check_cmd(arguments[n][0],"/usr/bin",0,1);//for tab feature
					if(tab_log ==NULL) fprintf(stderr,"command is not found\n");//if no command is found then tab_log return null
				}
				chdir(cwd);
				free(cwd);
#if debugarg
				//display all the arguments that is parsed to excute
				int b;
				for(b =0 ; b<sizeof(arguments[n]);b++) fprintf(stdout,"|%d %s|\n",b,arguments[n][b]);
#endif	

			
				//exit(1);
				execvp(*arguments[n],arguments[n]);
                                /*Missing code for parent process*/
                        default:
                                /* Parent process */
                                child_pids[n] = pid;
                                break;
                }
        }
        /*Parent process*/
        if (pid!=0 && pid!=-1) {

#if debugfork
                fprintf(stdout,"PARENT: my PID is %d\n", getpid());
#endif
                for (n=0; n<=cargc; n++){
                        int status = 0;
#if debugfork
                        fprintf(stdout,"PARENT: waiting for child %d (%d)\n", n, child_pids[n]);
#endif
                        int rc_pid = waitpid(child_pids[n], &status, 0);
#if debugfork
                        fprintf(stdout,"PARENT: Child: %d returned value is: %d\n", n, WEXITSTATUS(status));
#endif
                }
        }
}
	exit(1);//if the program quit this way, something is wrong

	
}
/***************************************all functions bodies should be written here***************************************************************************/
//parse the line of input, sepreate them into command catergory or argument and return both in pointers
int parse(char* input, char*** arguments,int* ampersandflag, int* para)
{
	int i=0;//counter for number of arguments9'
	para[i] = 0;
	int cmdnumber;//numbers that will be stored in parameters indicate numbers of arguments in each cmd entered
	char* temp = (char*) malloc(MAX_ARGS*sizeof(char));//temperary store the cmd entered
	
	arguments[i][para[i]]=strtok(input," ");//no \n for the strtok so that the \n will not be removed
	
	while(arguments[i][para[i]][strlen(arguments[i][para[i]])-1] != '\n'){//since the last bit/interge of the string is \0 therefore the \n is the one before the last bit
		//concurrent
		if(arguments[i][para[i]][strlen(arguments[i][para[i]])-1] == ';'){
			
			arguments[i][para[i]][strlen(arguments[i][para[i]])-1] = '\0';//replace ; with \0
			i++;
			para[i]=-1;//reinitialize
		}

		//the first token  is in the input is the command and the rest are the argument
		para[i]++;	
		arguments[i][para[i]] = strtok(NULL," ") ;
		
		
	}
	arguments[i][para[i]][strlen(arguments[i][para[i]])-1] = '\0';//replace \n with \0
	int m;
	for(m =0; m<=i;m++){
		arguments[m][para[m]+1]=NULL;
		if(arguments[m][para[m]][strlen(arguments[m][para[m]])-1] == '&'){
			arguments[m][para[m]][strlen(arguments[m][para[m]])-1] = '\0';//replace ; with \0
			ampersandflag[m]= 1;
		}
		else ampersandflag[m] = 0;
		//ampersand
	}
	
	return i;
}

//function that check the comand if it is in the directory
char* check_cmd(char* commands,char* dir, int depth, int further)
{		
	//return a string for tab feature and return one string if it is normal usage
	char* tab_log = (char*) malloc(MAX_LENGTH+1);//the +1 is for null and it can only show 20 similiar tab result
	// part of the code is come from http://blog.csdn.net/zhuyi2654715/article/details/7605051
	DIR *dirpath;

	int dirtybit=0;//indicate if the tab match has been more than one
	struct dirent *entry;
	struct stat statbuf;//stat buffer
	if((dirpath = opendir(dir))== NULL){
		fprintf(stderr, "the directory %s is not able to be open\n",dir);
		return;
	}

	chdir(dir);//change working directory

	while((entry = readdir(dirpath)) != NULL){//not the end of the directory
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {//check if it is still in the directory
			if(further==1){
				if (strcmp(entry->d_name, ".")==0||strcmp(entry->d_name, "..")==0) continue; 
				else if(strcmp(entry->d_name,commands)==1){//strcmp return 0 means it is a match
					check_cmd(commands,entry->d_name,depth+4,1);//go inside the folder
				}
				else if(strcmp(entry->d_name,commands)==0){
					tab_log = entry->d_name;
#if debugdir
					 fprintf(stdout,"%s is in foloder %s\n",entry->d_name,dir);
#endif

				}
			
			}
			else if(further == 0){//tab feature, it will compare the cmd with all the file in the working directory
				char* temp = (char*) malloc( strlen(commands)+1);
				int strcpycounter;
				if(strlen(commands) <= strlen(entry->d_name)){
					for(strcpycounter=0;strcpycounter<(strlen(commands)-1);strcpycounter++) temp[strcpycounter] = entry->d_name[strcpycounter];
					temp[strcpycounter] = '\t';
					if(strcmp(temp,commands)==0) {
							tab_log = entry->d_name;
							dirtybit++;
							//free up memory before quit
							free(temp);
					}


				}
				
			}
		}//s_ISDIR
	}//WHILE
	//change the working directory back to home and close it before return
	chdir("..");
	closedir(dirpath);
	if(further==0 && dirtybit!= 1) tab_log=NULL; //dirtybit will only be checked when it is a tab feature implementation
	
	return tab_log;//return nothing
	
}

void print(int(*p)[62])
{
int i,i2=0,j,j2=49;
for(i=0;i<20;i++)
{
fprintf(stdout," ");
for(j=0;j<62;j++)
{
if(i==i2||i==i2+1)
if(j>j2&&j<j2+13)
continue;
if(*(*(p+i)+j)==1||*(*(p+i)+j)==2)
fprintf(stdout,"*");
else if(*(*(p+i)+j)==3||*(*(p+i)+j)==4)
fprintf(stdout,"*");
else
fprintf(stdout," ");
}
fprintf(stdout," \n");
}
}
void star(int (*p)[62],int i,int j,int n)
{
int k,l,a,b;
for(k=0;k<18;k++,i++,j++)
{
*(*(p+i)+j)=n;
if(k>7&&k<11)
*(*(p+i)+j)=0;
}
i--,j--;
for(k=0;k<11;k++,i--,j-=4)
{
*(*(p+i)+j)=n;
if(k>4&&k<7)
*(*(p+i)+j)=0;
}
i++,j+=4;
for(k=0;k<47;k++,j++)
{
*(*(p+i)+j)=n;
}
j--;
for(k=0;k<11;k++,i++,j-=4)
{
*(*(p+i)+j)=n;
if(k>3&&k<6)
*(*(p+i)+j)=0;
}
i--,j+=4;
for(k=0;k<18;k++,i--,j++)
{
*(*(p+i)+j)=n;
if(k>6&&k<10)
*(*(p+i)+j)=0;
}
i++,j--;
a=i,b=j;
i++,j++;
for(k=1;k<17;k++,i++,j++)
for(l=j-1;*(*(p+i)+l)!=n;l--)
*(*(p+i)+l)=n+1;
i=a,j=b;
i++,j--;
for(k=1;k<17;k++,i++,j--)
for(l=j+1;*(*(p+i)+l)!=n;l++)
*(*(p+i)+l)=n+1;
} 



