#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
JNIEXPORT jint JNICALL Java_com_cs241honors_android_ocr_simple_local_solver_checkNumber(
		JNIEnv * env, jobject obj, jstring jpart){
	int * bool = malloc(sizeof(int));
	*bool = 0;
	const jbyte * part = (*env)->GetStringUTFChars(env, jpart, jboolean(bool));
	int i;
	for (i=0; i<strlen(part); i++){
		if(!isdigit(part[i])){
			return 0;
		}
	}
	return 1;
}
*/
int checkNumber(const char* part){
	int i;
	for (i=0; i<strlen(part); i++){
		if(!isdigit(part[i])){
			return 0;
		}
	}
	return 1;
}

/*
JNIEXPORT jstring JNICALL Java_com_cs241honors_android_ocr_simple_local_solver_substring(
		JNIEnv * env, jobject obj, jstring jstr, jint i, jint j){
	int * bool = malloc(sizeof(int));
	*bool = 0;
	const jbyte * str = (*env)->GetStringUTFChars(env,jstr,jboolean(bool));
	int len = j-i;
	int k;
	if(len==0){
		return "";
	}
	char* substrings = (char*)malloc((len+1)*sizeof(char));
	substrings[len] = '\0';
	for (k=0; k<len; k++){
		substrings[k]=str[i++];
	}
	return substrings;
}
*/
char* substring(const char* string, int i, int j){
	int len = j-i;
	int k;
	if(len==0){
		return "";
	}
	char* substrings = (char*)malloc((len+1)*sizeof(char));
	substrings[len] = '\0';
	for (k=0; k<len; k++){
		substrings[k]=string[i++];
	}
	return substrings;
}

JNIEXPORT jstring JNICALL Java_com_cs241honors_mathmagicka_MathMagickaActivity_simpleSolver(
	JNIEnv * env, jobject obj, jstring jequation){

	int * bool = malloc(sizeof(int));
	*bool = 0;
	const jbyte * equation = (*env)->GetStringUTFChars(env,jequation,(jboolean*)bool);
	int index,equalIndex;
	int flag_operate;
	int output;
	int flag_equal=1;
	char* part1="";
	char* part2="";
	char* part3="";

	if((index = (int)(strchr(equation,'+')-(char*)equation))>=0){
		flag_operate=1;
	}
	else if((index = (int)(strchr(equation,'-')-(char*)equation))>=0){
		flag_operate=2;
	}
	else if((index = (int)(strchr(equation,'*')-(char*)equation))>=0){
		flag_operate=3;
	}
	else if((index = (int)(strchr(equation,'/')-(char*)equation))>=0){
		flag_operate=4;
	}
	else{
		return (*env)->NewStringUTF(env,"Input cannot be solved locally");
	}

	part1 = substring(equation,0,index);
	if (part1 == ""){
		return (*env)->NewStringUTF(env,"Input cannot be solved locally");
	}

	if ((equalIndex = (int)(strchr(equation,'=')-(char*)equation))>=0){
		part2 = substring(equation, index+1, equalIndex);
		part3 = substring(equation, equalIndex+1, strlen(equation));
		if(part3==""){
			flag_equal=0;
		}
	}
	else{
		part2 = substring(equation, index+1, strlen(equation));
		if(part2==""){
			jstring re = (*env)->NewStringUTF(env,"Input cannot be solved locally");
			free(part1);
			return re;
		}
		flag_equal=0;
	}

	if(flag_equal==0){
		if(checkNumber(part1)==0||checkNumber(part2)==0){
			jstring re = (*env)->NewStringUTF(env,"Input cannot be solved locally");
			free(part1);
			free(part2);
			return re;
		}
		else{
			if(flag_operate==1){
				output = atoi(part1)+atoi(part2);
				char re[500];
				sprintf(re,"%d",output);
				return (*env)->NewStringUTF(env,re);
			}
			else if(flag_operate==2){
				output = atoi(part1)-atoi(part2);
				char re[500];
				sprintf(re,"%d",output);
				return (*env)->NewStringUTF(env,re);
			}
			if(flag_operate==3){
				output = atoi(part1)*atoi(part2);
				char re[500];
				sprintf(re,"%d",output);
				return (*env)->NewStringUTF(env,re);
			}

			else if(flag_operate==4){
				double output_1 =(double)atoi(part1)/(double)(atoi(part2));
				char re[500];
				sprintf(re,"%f",output_1);
				return (*env)->NewStringUTF(env,re);
			}
		}
	}

	else{
		int numberofx=0;
		int xindex;
		if(checkNumber(part1)==0){
			numberofx++;
			xindex=1;
		}
		if(checkNumber(part2)==0){
			numberofx++;
			xindex=2;
		}
		if(checkNumber(part3)==0){
			numberofx++;
			xindex=3;
		}
		if(numberofx!=1){
			jstring re = (*env)->NewStringUTF(env,"Input cannot be solved locally");
			free(part1);
			free(part2);
			free(part3);
			return re;
		}
		if(xindex==1){
			if(flag_operate==1){
				output = atoi(part3)-atoi(part2);
				double output_1 =(double)atoi(part1)/(double)(atoi(part2));
				char re[500];
				sprintf(re,"%s = %d",part1, output);
				return (*env)->NewStringUTF(env,re);
			}
			else if(flag_operate==2){
				output = atoi(part3)+atoi(part2);
				char re[500];
				sprintf(re,"%s = %d",part1, output);
				return (*env)->NewStringUTF(env,re);
			}
			if(flag_operate==4){
				output = atoi(part3)*atoi(part2);
				char re[500];
				sprintf(re,"%s = %d", part1, output);
				return (*env)->NewStringUTF(env,re);
			}

			else if(flag_operate==3){
				double output_1 =(double)atoi(part3)/(double)(atoi(part2));
				char re[500];
				sprintf(re,"%s = %f",part1, output_1);
				return (*env)->NewStringUTF(env,re);
			}
		}
		else if(xindex==2){
			if(flag_operate==1){
				output = atoi(part3)-atoi(part1);
				char re[500];
				sprintf(re,"%s = %d",part2,output);
				return (*env)->NewStringUTF(env,re);
			}
			else if(flag_operate==2){
				output = atoi(part3)+atoi(part1);
				char re[500];
				sprintf(re,"%s = %d",part2, output);
				return (*env)->NewStringUTF(env,re);
			}
			if(flag_operate==4){
				double output_1 =(double)atoi(part1)/(double)(atoi(part3));
				char re[500];
				sprintf(re,"%s = %f",part2, output_1);
				return (*env)->NewStringUTF(env,re);
			}

			else if(flag_operate==3){
				double output_1 =(double)atoi(part3)/(double)(atoi(part1));
				char re[500];
				sprintf(re,"%s = %f", part2, output_1);
				return (*env)->NewStringUTF(env,re);
			}
		}
		else if(xindex==3){
			if(flag_operate==1){
				output = atoi(part1)+atoi(part2);
				char re[500];
				sprintf(re,"%s = %d",part3, output);
				return (*env)->NewStringUTF(env,re);
			}
			else if(flag_operate==2){
				output = atoi(part1)-atoi(part2);
				char re[500];
				sprintf(re,"%s = %d", part3, output);
				return (*env)->NewStringUTF(env,re);
			}
			if(flag_operate==3){
				output = atoi(part1)*atoi(part2);
				char re[500];
				sprintf(re,"%s = %d",part3, output);
				return (*env)->NewStringUTF(env,re);
			}

			else if(flag_operate==4){
				double output_1 =(double)atoi(part1)/(double)(atoi(part2));
				char re[500];
				sprintf(re,"%s = %f",part3,output_1);
				return (*env)->NewStringUTF(env,re);
			}
		}
	}

	free(part1);
	free(part2);
	if(part3!=""){
		free(part3);
	}
	return (*env)->NewStringUTF(env,"No computation done");
}

