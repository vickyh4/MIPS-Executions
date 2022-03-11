// Assignment 2 20T2, Vicky Hu, z5255592
// 06/08/20
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Defining maximum lines of mips code as 1000 and maximum number of registers to be 32
#define MAX_INPUT 1000
#define MAX_REGISTERS 32

int readFile(long **mips, char *filename);
void processHex(char *filename, long **registers);
void initialiseRegisters(long **registers);
void printRegisters(long **registers);
void printOutput();
void outputCharacter(FILE *fp, char character);
void outputInteger(FILE *fp, long integer);
void outputUnknown(FILE *fp, int id);
void printSpace(int index);

int main(int argc, char* argv[]) {
    long *registers = malloc(MAX_REGISTERS*sizeof(long));
    initialiseRegisters(&registers);
    
    processHex(argv[1], &registers);
    printOutput();
    printRegisters(&registers);

    free(registers);
    return 0;
}
// After the program is run, prints the output of the program
void printOutput() {
    // Open "output.txt" to read the output saved 
    FILE *fp;
    fp = fopen("output.txt", "r");
    // If the file does not exist then print error
    if (fp == NULL)
        printf("No output file found\n");

    // If file does exist, print the output
    printf("Output\n");
    // A loop to get lines out of output until EOF
    do {
        int c = fgetc(fp);

        if (feof(fp))
            break;

        printf("%c", c);
    } while(1);

    // Close the file at the end
    fclose(fp);
}

// Function to print out register values after execution
void printRegisters(long **registers) {
    printf("Registers After Execution\n");
    // Print (*registers) after execution
    // Starting fron i = 1 as (*registers) $0 is always 0
    for (int i =1; i < MAX_REGISTERS; i++) {
        if ((*registers)[i] == 0) continue;
        // If conditions which account for spacing for double digit and single digits
        if (i < 10) {
            printf("$%d  = %ld\n", i, (*registers)[i]);
        } else {
            printf("$%d = %ld\n", i, (*registers)[i]);
        }
    }
}
// Initialise all 32 registers to be 0
void initialiseRegisters(long **registers) {
    for (int i=0; i < MAX_REGISTERS; i++) 
        (*registers)[i]=0;
}
// Function to print integer output to the output.txt file
void outputInteger(FILE *fp, long integer) {
    fprintf(fp, "%ld", integer);

}
// Function to print character output to the output.txt file
void outputCharacter(FILE *fp, char character) {
    fprintf(fp, "%c", character);
}
// Function to print error output to the output.txt file
void outputUnknown(FILE *fp, int id) {
    fprintf(fp, "Unknown system call: %d\n", id);
}
// Function which prints a space for program line indenting
void printSpace(int index) {
    if (index < 10) {
        printf(" ");
    }
}
// Function which prints the Hex code provided to mips, runs execution and stores register/output values  
void processHex(char *filename, long **registers) {
    // Open file to store output as the execution occurs
    FILE *fp;
    fp = fopen("output.txt", "w");
    // Allocating memory for MIPS array 
    long *mips = malloc(MAX_INPUT*sizeof(int));
    // An int to store the number of lines of mips code there are
    int lines;
    // A boolean to check whether the line of mips has been printed already
    bool printLog[MAX_INPUT] = {false};

    lines = readFile(&mips, filename);
    // An exit flag to check whether the execution has called the syscall exit
    bool exitFlag = false;

    //Initialise variables to help print out values 
    int s, t ,d ,I;
    int i = 0;
    // First if condition distinguishes if the command begins with 6 zeros 
    // Inside this condition analyses the end 6 to determine which command
    printf("Program\n");
    while (i < lines) {
        // If condition to check if the left 6 binary is 000000
        if ((mips[i] >> 26) == 0) {
            // All hex codes beginning with 000000 have the same formatting
            // Store values of s, t, d 
            s = (mips[i] & 0x3E00000) >> 21;
            t = (mips[i] & 0x1F0000) >> 16;
            d = (mips[i] & 0xF800) >> 11;
            int mask = 0x3F;
            mask = mask & mips[i];
            
            if ((mask ^ 0x20) == 0) {
                // add $d = $s + $t
                // Print mips line of code if print log is false
                if (!printLog[i]) {
                    printSpace(i);
                    printf(" %d: add  $%d, $%d, $%d\n", i, d, s, t);
                    // Once printed, set printlog to true to prevent printing again
                    printLog[i] = true;
                }
                // Check if the execution has exited before continueing with execution
                if (!exitFlag && d !=0)
                    (*registers)[d] = (*registers)[s] + (*registers)[t];
                i++;
                continue;
            } else if (((mips[i] << 26) ^ 0x88000000) == 0) {
                //sub $d = $s - $t
                // Print mips line of code if print log is false
                if (!printLog[i]) {
                    printSpace(i);
                    printf(" %d: sub  $%d, $%d, $%d\n", i, d, s, t);
                    // Once printed, set printlog to true to prevent printing again
                    printLog[i] = true;
                }
                // Check if the execution has exited before continueing with execution
                if (!exitFlag && d !=0)
                    (*registers)[d] = (*registers)[s] - (*registers)[t];
                i++;
                continue;
            } else if (((mips[i] << 26) ^ 0x90000000) == 0) {
                //and $d = $s & $t
                // Print mips line of code if print log is false
                if (!printLog[i]) {
                    printSpace(i);
                    printf(" %d: and  $%d, $%d, $%d\n", i, d, s, t);
                    // Once printed, set printlog to true to prevent printing again
                    printLog[i] = true;
                }         
                // Check if the execution has exited before continueing with execution
                if (!exitFlag && d != 0)
                    (*registers)[d] = (*registers)[s] & (*registers)[t];
                i++;
                continue;
            } else if (((mips[i] << 26) ^ 0x94000000) == 0) {
                //or $d, $s, $t
                // Print mips line of code if print log is false
                if (!printLog[i]) {
                    printSpace(i);
                    printf(" %d: or  $%d, $%d, $%d\n", i, d, s, t);
                    // Once printed, set printlog to true to prevent printing again
                    printLog[i] = true;
                }     
                // Check if the execution has exited before continueing with execution 
                if (!exitFlag && d != 0)
                    (*registers)[d] = (*registers)[s] | (*registers)[t];
                i++;
                continue;
            // Compare to hex number instead
            } else if ((mask ^ 0x2A) == 0) {
                //slt $d, $s, $t
                // Print mips line of code if print log is false
                if (!printLog[i]) {
                    printSpace(i);
                    printf(" %d: slt  $%d, $%d, $%d\n", i, d, s, t);
                    // Once printed, set printlog to true to prevent printing again
                    printLog[i] = true;
                }  
                // Check if the execution has exited before continueing with execution
                if (!exitFlag && d != 0) {
                    if ((*registers)[s] < (*registers)[t]) {
                        (*registers)[d] = 1;
                    } else {
                        (*registers)[d] = 0;
                    }
                }
                i++;
                continue;
            } else if ((mips[i] ^ 12) == 0) {
                //syscall
                // Print mips line of code if print log is false
                if (!printLog[i]) {
                    printSpace(i);
                    printf(" %d: syscall\n", i);
                    // Once printed, set printlog to true to prevent printing again
                    printLog[i] = true;
                }  
                // Check if the execution has exited before continueing with execution
                if (!exitFlag) {
                    // Checks $v0 for which execution for syscall is called and checks $a0 register 
                    if ((*registers)[2] == 1) {
                        outputInteger(fp, (*registers)[4]);
                    } else if ((*registers)[2] == 10) {
                        exitFlag = true;
                    } else if ((*registers)[2] == 11) {
                        outputCharacter(fp, (char)(*registers)[4]);
                    } else {
                        outputUnknown(fp, (*registers)[2]);
                        exitFlag = true;
                        
                    }
                }
                i++;
                continue;
            }
        }
        // 8 other if conditions analyse the rest 
        if (((mips[i] >> 26) ^ 28) == 0) {
            //mul $d = $s * $t
            
            s = (mips[i] & 0x3E00000) >> 21;
            t = (mips[i] & 0x1F0000) >> 16;
            d = (mips[i] & 0xF800) >> 11;
            // Print mips line of code if print log is false
            if (!printLog[i]) {
                printSpace(i);
                printf(" %d: mul  $%d, $%d, $%d\n", i, d, s, t);
                // Once printed, set printlog to true to prevent printing again
                printLog[i] = true;
            }  
            // Check if the execution has exited before continueing with execution
            if (!exitFlag && d !=0)
                (*registers)[d] = (*registers)[s] * (*registers)[t];
            i++;
            continue;        
        }
        // The following hex codes have the same format, hence s,t,I can be initialised first
        s = (mips[i] & 0x3E00000) >> 21;
        t = (mips[i] & 0x1F0000) >> 16;
        I = (mips[i] & 0xFFFF);
        // Checks if the immediate number is a negative
        if ((I >> 15) == 1) {
            I = -(~mips[i] & 0x7FFF)-1;
        }

        if (((mips[i] >> 26) ^ 4) == 0) {
            //beq $s, $t, I
            // Print mips line of code if print log is false
            if (!printLog[i]) {
                printSpace(i);
                printf(" %d: beq  $%d, $%d, %d\n", i, s, t, I);
                // Once printed, set printlog to true to prevent printing again
                printLog[i] = true;
            }
            // If s equals t then branch to relevant line in mips array by adding i and I
            if (!exitFlag) {
                if((*registers)[s] == (*registers)[t]) {
                    i += I;
                    continue;
                }
            }
            // If s does not equal t then continue to next mips line
            i++;
            continue;        
        } else if (((mips[i] >> 26) ^ 5) == 0) {
            //bne $s, $t, I
            // Print mips line of code if print log is false
            if (!printLog[i]) {
                printSpace(i);
                printf(" %d: bne  $%d, $%d, %d\n", i, s, t, I);
                // Once printed, set printlog to true to prevent printing again
                printLog[i] = true;
            }
            // Check if the execution has exited before continueing with execution
            if (!exitFlag) {
                // If s doesn't equal t then branch to relevant line in mips array by adding i and I 
                if((*registers)[s] != (*registers)[t]) {
                    i += I;
                    continue;
                }
            }
            // If s does equal t then continue to next mips line
            i++;
            continue;                
        } else if (((mips[i] >> 26) ^ 8) == 0) {
            //addi $t = $s + I
            // Print mips line of code if print log is false
            if (!printLog[i]) {
                printSpace(i);
                printf(" %d: addi $%d, $%d, %d\n", i, t, s, I);
                // Once printed, set printlog to true to prevent printing again
                printLog[i] = true;
            }
            // Check if the execution has exited before continueing with execution
            if (!exitFlag && t !=0) 
                (*registers)[t] = (*registers)[s] + I;
            i++;
            continue;     
             
        } else if (((mips[i] >> 26) ^ 10) == 0) {
            //slti $t = ($s < I)
            // Print mips line of code if print log is false
            if (!printLog[i]) {
                    printSpace(i);
                    printf(" %d: slti  $%d, $%d, %d\n", i, t, s, I);
                    // Once printed, set printlog to true to prevent printing again
                    printLog[i] = true;
                }
            // Check if the execution has exited before continueing with execution    
            if (!exitFlag && t != 0)
                (*registers)[t] = ((*registers)[s] < I);
            i++;
            continue;        
        } else if (((mips[i] >> 26) ^ 12) == 0) {
            //andi $t = $s & I
            // Print mips line of code if print log is false
            if (!printLog[i]) {
                printSpace(i);
                printf(" %d: andi  $%d, $%d, %d\n", i, t, s, I);
                // Once printed, set printlog to true to prevent printing again
                printLog[i] = true;
            }
            // Check if the execution has exited before continueing with execution
            if (!exitFlag && t!=0)
                (*registers)[t] = (*registers)[s] & I;
            i++;
            continue;        
        } else if (((mips[i] >> 26) ^ 13) == 0) {
            //ori $t = $s | I
            // Print mips line of code if print log is false
            if (!printLog[i]) {
                printSpace(i);
                printf(" %d: ori  $%d, $%d, %d\n", i, t, s, I);
                // Once printed, set printlog to true to prevent printing again
                printLog[i] = true;
            }
            // Check if the execution has exited before continueing with execution
            if (!exitFlag && t!=0)
                (*registers)[t] = (*registers)[s]|I;
            i++;
            continue;        
        } else if (((mips[i] >> 26) ^ 15) == 0) {
            //lui $t = I << 16
            // Print mips line of code if print log is false
            if (!printLog[i]) {
                printSpace(i);
                printf(" %d: lui  $%d, %d\n", i, t, I);
                // Once printed, set printlog to true to prevent printing again
                printLog[i] = true;
            }
            // Check if the execution has exited before continueing with execution
            if (!exitFlag && t!=0)
                (*registers)[t] = I << 16;
            i++;
            continue;        
        }
        // If the loop reaches the end, the line is not a command and exit
        //i++;

        printf("Invalid instruction code\n");
        break;
    }
    fclose(fp);
    free(mips);
    
}


int readFile(long **mips, char *filename) {
    // Scans in hex code input and stores them in an array
    // Array to store lines of mips code
    int lines=0;
    FILE *fp;

    fp = fopen(filename,"r");

    int buffer;

    if (fp == NULL)
        printf("Could not open file\n");


    // //Scan in hex codes given into the mips array
    for (lines=0; fscanf(fp, "%x", &buffer) != EOF; lines++) {
        (*mips)[lines] = buffer;
    }

    fclose(fp);
    return lines;
}