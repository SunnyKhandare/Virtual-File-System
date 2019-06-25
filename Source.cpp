#include"headerfile.hpp"

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;


void InitialiseSuperBlock()   //InitialiseSuperBlock
{
	int i = 0;
	while (i<MAXINODE)
	{
		UFDTArr[i].ptrfiletable = NULL;
		i++;
	}
	SUPERBLOCKobj.TotalInodes = MAXINODE;
	SUPERBLOCKobj.FreeInode = MAXINODE;
}

void CreateDILB()             //CreateDILB
{
	PINODE newn = NULL;
	PINODE temp = head;
	int i = 1;
	while (i <= MAXINODE)
	{
		newn = (PINODE)malloc(sizeof(INODE));
		newn->LinkCount = newn->ReferenceCount = 0;
		newn->Buffer = NULL;
		newn->FileType = newn->Filesize = 0;
		newn->next = NULL;
		newn->InodeNumber = i;
		if (temp == NULL)
		{
			head = newn;
			temp = head;
		}
		else
		{
			temp->next = newn;
			temp = temp->next;
		}
		i++;
	}
}


void ls_file()       //ls_file
{
	PINODE temp = head;
	if (SUPERBLOCKobj.FreeInode == MAXINODE)
	{
		printf("ERROR:There are no file\n");
		return;
	}

	printf("\n----------------------------------------------------------------------------------------\n");
	while (temp != NULL)
	{
		if (temp->FileType != 0)
		{
			printf("File Name:%s\n", temp->FileName);
			printf("Inode Number:%d\n", temp->InodeNumber);
			printf("File Size:%d\n", temp->FileActualSize);
			printf("Link count:%d\n", temp->LinkCount);
		}
		temp = temp->next;
	}
	printf("*******************************************************************************************\n");
}


void CloseAllFile()        //closeAllFile
{
	int i = 0;
	while (i<MAXINODE)
	{
		if (UFDTArr[i].ptrfiletable != NULL)
		{
			UFDTArr[i].ptrfiletable->readoffset = 0;
			UFDTArr[i].ptrfiletable->writeoffset = 0;
			UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount--;
		}
		i++;
	}
}


int stat_file(char *name)          //stat_file
{
	PINODE temp = head;
	if (name == NULL)
	{
		return -1;
	}

	while (temp != NULL)
	{
		if (_stricmp(name, temp->FileName) == 0)
			break;
		temp = temp->next;
	}

	if (temp == NULL)
	{
		return -2;
	}

	printf("--------------Statistical Informatiom about file-----------\n");
	printf("File Name:%s\n", temp->FileName);
	printf("Inode Number:%d\n", temp->InodeNumber);
	printf("Actual File Size:%d\n", temp->FileActualSize);
	printf("Link count:%d\n", temp->LinkCount);
	printf("File Size:%d\n", temp->Filesize);
	printf("Reference Count:%d\n", temp->ReferenceCount);
	if (temp->Permission == 1)
	{
		printf("File Permission :Read only\n");
	}
	else if (temp->Permission == 2)
	{
		printf("File permission :Write only\n");
	}
	else if (temp->Permission == 3)
	{
		printf("File Permission :Read & Write \n");
	}
	else
		;
	printf("\n*************************************************************************************\n");
	return 0;
}

int fstat_file(int fd)        ///fstat_file
{
	PINODE temp = head;
	if (fd<0)
	{
		return -1;
	}
	if (UFDTArr[fd].ptrfiletable == NULL)
	{
		return -2;
	}
	temp = UFDTArr[fd].ptrfiletable->ptrinode;

	printf("--------------Statistical Informatiom about file-----------\n");
	printf("File Name:%s\n", temp->FileName);
	printf("Inode Number:%d\n", temp->InodeNumber);
	printf("Actual File Size:%d\n", temp->FileActualSize);
	printf("Link count:%d\n", temp->LinkCount);
	printf("File Size:%d\n", temp->Filesize);
	printf("Reference Count:%d\n", temp->ReferenceCount);
	if (temp->Permission == 1)
	{
		printf("File Permission :Read only\n");
	}
	else if (temp->Permission == 2)
	{
		printf("File permission :Write only\n");
	}
	else if (temp->Permission == 3)
	{
		printf("File Permission :Read & Write \n");
	}
	else
		;
	printf("\n*************************************************************************************\n");
	return 0;
}

int GetFDFromName(char *name)              //GetFDFromName
{
	int i = 0;
	while (i<MAXINODE)
	{
		if (UFDTArr[i].ptrfiletable != NULL)
		{
			if ((_stricmp(UFDTArr[i].ptrfiletable->ptrinode->FileName, name) == 0))
				break;
		}
		i++;
	}
	if (i == 50)
		return -1;
	else
		return i;
}



int CloseFileByName(char * name)              //CloseFileByName
{
	int i = 0;
	i = GetFDFromName(name);
	if (i == -1)
	{
		return -1;
	}
	UFDTArr[i].ptrfiletable->readoffset = 0;
	UFDTArr[i].ptrfiletable->writeoffset = 0;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
	return 0;
}


int rm_File(char *name)                    //rm_File
{
	int fd = 0;
	fd = GetFDFromName(name);
	if (fd == -1)
	{
		return -1;
	}
	(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;
	if (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
	{
		UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
		free(UFDTArr[fd].ptrfiletable);
	}
	UFDTArr[fd].ptrfiletable = NULL;
	(SUPERBLOCKobj.FreeInode)++;
	return 1;
}


void man(char *name)                         //man
{
	if (name == NULL)
		return;
	if (_stricmp(name, "create") == 0)
	{
		printf("Description:Used to create new regular file\n");
		printf("Usage:create File_name permission\n");
	}
	else if (_stricmp(name, "read") == 0)
	{
		printf("Description:Used to read data fromm regular file\n");
		printf("usage:read file_name no_of_bytes_to_read\n");
	}
	else if (_stricmp(name, "write") == 0)
	{
		printf("Description:used to write data from regular file\n");
		printf("Usage:write File_name\n After this enter the data that we want to write\n");
	}
	else if (_stricmp(name, "ls") == 0)
	{
		printf("Description:used to list all information of files\n");
		printf("usage:ls\n");
	}
	else if (_stricmp(name, "stat") == 0)
	{
		printf("Description:used to display information of file\n");
		printf("usage:stat file_name\n");
	}
	else if (_stricmp(name, "fstat") == 0)
	{
		printf("Description:used to display information of file\n");
		printf("usgae:stat file_descriptor\n");
	}
	else if (_stricmp(name, "truncate") == 0)
	{
		printf("Description:used to remove data from file\n");
		printf("Usage:truncate file_name\n");
	}
	else if (_stricmp(name, "open") == 0)
	{
		printf("Description:Used to open existing file\n");
		printf("Usage:open file_name mode\n");
	}
	else if (_stricmp(name, "close") == 0)
	{
		printf("Description:Used to close opened file\n");
		printf("Usage:close file_name\n");
	}
	else if (_stricmp(name, "closeall") == 0)
	{
		printf("Description:used to close all opened file\n");
		printf("Usage:closeall\n");
	}
	else if (_stricmp(name, "lseek") == 0)
	{
		printf("Description:used to change file offset\n");
		printf("usage:lseek file_name changeinoffset startpoint\n");
	}
	else if (_stricmp(name, "rm") == 0)
	{
		printf("description:Used to delete file\n");
		printf("usage:rm_file_name\n");
	}
	else
	{
		printf("ERROR:No manual entry available\n");
	}
}

void DisplayHelp()	//DisplayHelp
{
	printf("ls : To list out all files\n");
	printf("clear : To clear console\n");
	printf("open : To open the file\n");
	printf("close : To close the file\n");
	printf("closeall : To close all opened files\n");
	printf("read : To read the contents from file\n");
	printf("write : To write the contents into file\n");
	printf("exit : To teminate file system\n");
	printf("stat : To display information of file using name\n");
	printf("fstat : To display information of file using file descriptor\n");
	printf("truncate : To remove all data from file\n");
	printf("rm : To delete the file\n");
}

int WriteFile(int fd, char *arr, int isize)             //WriteFile
{
	if (((UFDTArr[fd].ptrfiletable->mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != READ + WRITE))
	{
		return -1;
	}

	if (((UFDTArr[fd].ptrfiletable->ptrinode->Permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->Permission) != READ + WRITE))
	{
		return -1;
	}

	if ((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE)
	{
		return -2;
	}

	if ((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR)
	{
		return -3;
	}

	strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset), arr, isize);
	(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + isize;
	(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + isize;
	return isize;
}


PINODE Get_Inode(char  * name)            //Get_Inode
{
	PINODE temp = head;
	if (name == NULL)
	{
		return NULL;
	}
	while (temp != NULL)
	{
		if (_stricmp(name, temp->FileName) == 0)
			break;
		temp = temp->next;
	}
	return temp;
}

int CreateFile(char *name, int permission)                //CreateFile
{
	int i = 0;
	PINODE temp = head;
	if ((name == NULL) || (permission == 0) || (permission>3))
	{
		return -1;
	}
	if (SUPERBLOCKobj.FreeInode == 0)
	{
		return -2;
	}
	if (Get_Inode(name) != NULL)
	{
		return -3;
	}

	(SUPERBLOCKobj.FreeInode)--;
	while (temp != NULL)
	{
		if (temp->FileType == 0)
			break;
		temp = temp->next;
	}
	while (i<MAXINODE)
	{
		if (UFDTArr[i].ptrfiletable == NULL)
			break;
		i++;
	}

	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if (UFDTArr[i].ptrfiletable == NULL)
	{
		return -4;
	}

	UFDTArr[i].ptrfiletable->count = 1;
	UFDTArr[i].ptrfiletable->mode = permission;
	UFDTArr[i].ptrfiletable->readoffset = 0;
	UFDTArr[i].ptrfiletable->writeoffset = 0;
	UFDTArr[i].ptrfiletable->ptrinode = temp;
	strcpy_s(UFDTArr[i].ptrfiletable->ptrinode->FileName, name);
	UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
	UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->Filesize = MAXFILESIZE;
	UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
	UFDTArr[i].ptrfiletable->ptrinode->Permission = permission;
	UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);
	memset(UFDTArr[i].ptrfiletable->ptrinode->Buffer, 0, MAXFILESIZE);
	return i;
}

int OpenFile(char * name, int mode)             //OpenFile
{
	int i = 0;
	PINODE temp = NULL;
	if ((name == NULL) || (mode <= 0))
	{
		return -1;
	}
	temp = Get_Inode(name);
	if (temp == NULL)
	{
		return -2;
	}
	if (temp->Permission<mode)
	{
		return -3;
	}
	while (i<MAXINODE)
	{
		if (UFDTArr[i].ptrfiletable == NULL)
		{
			break;
		}
		i++;

	}
	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if (UFDTArr[i].ptrfiletable == NULL)
	{
		return -1;
	}
	UFDTArr[i].ptrfiletable->count = 1;
	UFDTArr[i].ptrfiletable->mode = mode;
	if (mode == READ + WRITE)
	{
		UFDTArr[i].ptrfiletable->readoffset = 0;
		UFDTArr[i].ptrfiletable->writeoffset = 0;
	}
	else if (mode == READ)
	{
		UFDTArr[i].ptrfiletable->readoffset = 0;
	}
	else if (mode == WRITE)
	{
		UFDTArr[i].ptrfiletable->writeoffset = 0;
	}
	UFDTArr[i].ptrfiletable->ptrinode = temp;
	(UFDTArr[i].ptrfiletable->count)++;

	return i;
}

int ReadFile(int fd, char * arr, int isize)              //ReadFile
{
	int read_size = 0;
	if (UFDTArr[fd].ptrfiletable == NULL)
	{
		return -1;
	}
	if (UFDTArr[fd].ptrfiletable->mode != READ && UFDTArr[fd].ptrfiletable->mode != READ + WRITE)
	{
		return -2;
	}
	if ((UFDTArr[fd].ptrfiletable->ptrinode->Permission != READ) && (UFDTArr[fd].ptrfiletable->ptrinode->Permission != READ + WRITE))
	{
		return -2;
	}

	if (UFDTArr[fd].ptrfiletable->readoffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
	{
		return -3;
	}

	if (UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
	{
		return -4;
	}

	read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - (UFDTArr[fd].ptrfiletable->readoffset);
	if (read_size<isize)
	{
		strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset), read_size);
		UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size;
		return read_size;
	}
	else
	{
		strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset), isize);
		UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + isize;
		return isize;
	}
}

int LseekFile(int fd, int size, int from)               //LseekFile
{
	if ((fd<0) || (from>2))
	{
		return -1;
	}
	if (UFDTArr[fd].ptrfiletable == NULL)
	{
		return -1;
	}
	if ((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == READ + WRITE))
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].ptrfiletable->readoffset) + size)>(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->readoffset) + size)<0)
			{
				return -1;
			}
			UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + size;
		}
		else if (from == START)
		{
			if (size>(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
			{
				return -1;
			}
			if (size<0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->readoffset) = size;
		}
		else if (from == END)
		{
			if ((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size >MAXFILESIZE)
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->readoffset) + size)<0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
		}
		else
			;
	}
	else if (UFDTArr[fd].ptrfiletable->mode == WRITE)
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].ptrfiletable->writeoffset) + size)>MAXFILESIZE)
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->writeoffset) + size)<0)
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->writeoffset) + size)>(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
			{
				(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
			}
			(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
		}
		else if (from == START)
		{
			if (size>MAXFILESIZE)
			{
				return -1;
			}
			if (size<0)
			{
				return -1;
			}

			if (size>(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
			{
				(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = size;
			}
			(UFDTArr[fd].ptrfiletable->writeoffset) = size;
		}
		else if (from == END)
		{
			if ((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size >MAXFILESIZE)
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->writeoffset) + size)<0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
		}
		else
			;
	}
	else
		;

	return 1;

}

int truncate_File(char * name)
{
	int fd = GetFDFromName(name);
	if (fd == -1)
		return -1;
	memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer, 0, 1024);
	UFDTArr[fd].ptrfiletable->readoffset = 0;
	UFDTArr[fd].ptrfiletable->writeoffset = 0;
	UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;

	return 1;
}


int main()
{
	char * ptr = NULL;
	int ret = 0;
	int fd = 0;
	int count = 0;
	char command[4][80];
	char str[80];
	char arr[MAXFILESIZE];


	InitialiseSuperBlock();
	CreateDILB();

	while (1)
	{
		fflush(stdin);
		strcpy_s(str, "");
		printf("\n Virtual File System\n");
		fgets(str, 80, stdin);
		count = sscanf(str, "%s%s%s%s", command[0], command[1], command[2], command[3]);

		if (count == 1)
		{
			if (_stricmp(command[0], (const char *)"ls") == 0)
			{
				ls_file();
			}
			else if (_stricmp(command[0], "closeall") == 0)
			{
				CloseAllFile();
				printf("All files close Successfully\n");
				continue;
			}
			else if (_stricmp(command[0], "clear") == 0)
			{
				system("cls");
				continue;
			}
			else if (_stricmp(command[0], "help") == 0)
			{
				DisplayHelp();
				continue;
			}
			else if (_stricmp(command[0], "exit") == 0)
			{
				printf("Terminating the Virtual File System\n");
				break;
			}
			else
			{
				printf("Error:command not found\n");
			}
		}

		else if (count == 2)
		{

			if (_stricmp(command[0], "stat") == 0)
			{
				ret = stat_file(command[1]);
				if (ret == -1)
				{
					printf("ERROR:Incorrect Parameter\n");
				}
				if (ret == -2)
				{
					printf("ERROR:There is no such File\n");
				}
				continue;
			}

			else if (_stricmp(command[0], "fstat") == 0)
			{
				ret = fstat_file(atoi(command[1]));
				if (ret == -1)
				{
					printf("ERROR:Incoorect Parameter\n");
				}
				if (ret == -2)
				{
					printf("ERROR:There is no such File\n");
				}
				continue;
			}

			else if (_stricmp(command[0], "close") == 0)
			{
				ret = CloseFileByName(command[1]);
				if (ret == -1)
				{
					printf("ERROR:There is no such file\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "rm") == 0)
			{
				ret = rm_File(command[1]);
				if (ret == -1)
				{
					printf("ERROR:There is no such File\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "man") == 0)
			{
				man(command[1]);
				continue;
			}
			else if (_stricmp(command[0], "write") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR:Incorrect parameter\n");
					continue;
				}
				printf("Enter the data :\n");
				scanf("%[^\n]", arr);
				ret = strlen(arr);
				if (ret == 0)
				{
					printf("ERROR:Incorrect parameter\n");
					continue;
				}
				ret = WriteFile(fd, arr, ret);
				if (ret == -1)
				{
					printf("ERROR:Permission Denied\n");
				}
				if (ret == -2)
				{
					printf("ERROR:Ther is no sufficient memory to write\n");
				}
				if (ret == -3)
				{
					printf("ERROR:It is Not a regular file\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "truncate") == 0)
			{
				ret = truncate_File(command[1]);
				if (ret == -1)
				{
					printf("ERROR:Incorrect Paramete\n");
				}
				continue;
			}
			else
			{
				printf("\nERROR:command not found\n");
				continue;
			}
		}

		else if (count == 3)
		{
			if (_stricmp(command[0], "create") == 0)
			{
				ret = CreateFile(command[1], atoi(command[2]));
				if (ret >= 0)
				{
					printf("File Is Successfully created with file Descriptor\n");
				}
				if (ret == -1)
				{
					printf("ERROR:Incorrect parametrs\n");
				}
				if (ret == -2)
				{
					printf("ERROR:There is no Inodes\n");
				}
				if (ret == -3)
				{
					printf("ERROR:File already exists\n");
				}
				if (ret == -4)
				{
					printf("ERROR:Memory Allocation Failure\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "open") == 0)
			{
				ret = OpenFile(command[1], atoi(command[2]));
				if (ret >= 0)
				{
					printf("File Is successfully opened with file descriptors\n");
				}
				if (ret == -1)
				{
					printf("ERROR:Incorrect Parametrs\n");
				}
				if (ret == -2)
				{
					printf("ERROR:File Not present]n");
				}
				if (ret == -3)
				{
					printf("ERROR:permission Denied\n");
				}
				continue;
			}

			else if (_stricmp(command[0], "read") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR:Incorrect parameter\n");
					continue;
				}
				ptr = (char *)malloc(sizeof(atoi(command[2])) + 1);
				if (ptr == NULL)
				{
					printf("ERROR:Memory Allocation Failure\n");
					continue;
				}

				ret = ReadFile(fd, ptr, atoi(command[2]));
				if (ret == -1)
				{
					printf("ERROR:File not eisting\n");
				}
				if (ret == -2)
				{
					printf("ERROR:Permission Denied\n");
				}
				if (ret == -3)
				{
					printf("ERROR:Reached at the end of file\n");
				}
				if (ret == -4)
				{
					printf("ERROR:It is not regular file\n");
				}
				if (ret == 0)
				{
					printf("ERROR:File Empty\n");
				}
				if (ret>0)
				{
					_write(2, ptr, ret);
				}
				continue;
			}
			else
			{
				printf("\nERROR:command not found\n");
				continue;
			}
		}

		else if (count == 4)
		{
			if (_stricmp(command[0], (const char *)"lseek") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR:Incorrect parameters\n");
					continue;
				}
				ret = LseekFile(fd, atoi(command[2]), atoi(command[3]));

				if (ret == -1)
				{
					printf("ERROR:unable to perform lseek\n");
				}
			}
			else
			{
				printf("\nERROR:command not found\n");
				continue;
			}
		}
		else
		{
			printf("\nERROR:command not found\n");
			continue;
		}
	}
	return 0;
}
