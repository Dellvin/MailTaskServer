//
// Created by Daniil Tchyorny on 04.09.2020.
//

#ifndef ENTERINGTASK_SERVER_CONSTEXP_H
#define ENTERINGTASK_SERVER_CONSTEXP_H
constexpr char HELLO_MESSAGE[] = "HELLO, PLEASE ENTER YOUR LOGIN: ";

constexpr char BREAK_LINE[]="----------------------------------------------------\n\n";

constexpr char DELETE_ERROR[]="----------------------------------------------------\n\n"
                              "SORRY BUT IT IS NOT YOUR ROOM!\n"
                              "shame on you!!!!!!!!!!!!!!!!!!\n\n"
                              "----------------------------------------------------\n\n";

constexpr char JOIN_ERROR[]="----------------------------------------------------\n\n"
                            "SORRY, BUT THIS ROOM DOES NOT EXIST\n"
                            "try to use '#show'\n"
                            "or create your own room '#create'\n\n"
                            "----------------------------------------------------\n\n";

constexpr char WRONG_LOGIN[] =  "----------------------------------------------------\n\n"
                                "YOU USED WRONG SYMBOL IN LOGIN\n"
                                "please use only that: a-z, A-Z, 0-9, _\n\n"
                                "----------------------------------------------------\n\n";

constexpr char UNDEFINE_MESSAGE[] = "----------------------------------------------------\n\n"
                                    "sorry, but we do not understand\n"
                                    "what you wanted to say\n"
                                    "please try to type '#help'(\n\n"
                                    "----------------------------------------------------\n";

constexpr char HELP_MESSAGE[] = "----------------------------------------------------\n\n"
                                "#create ${...} - create the room\n"
                                "#join ${...} - join to some room\n"
                                "#show - show the rooms list\n"
                                "#exit - exit the programm\n"
                                "#leave - leave the room\n"
                                "#delete - delete the room\n"
                                "#member - show the members list in current room\n"
                                "#help - you are here)\n\n"
                                "----------------------------------------------------\n";
constexpr char EXIT_MESSAGE[] = "----------------------------------------------------\n\n"
                                "BYE, I HOPE WE SEE YOU AGAIN!\n\n"
                                "----------------------------------------------------\n";


constexpr char TOO_MUCH_LOGIN[] = "----------------------------------------------------\n\n"
                                "LOGIN ERROR, TOO MUCH WORDS!\n\n"
                                "----------------------------------------------------\n";

constexpr char USED_LOGIN[] = "----------------------------------------------------\n\n"
                                  "SORRY, THIS NICKNAME HAS ALREADY USED!\n\n"
                                  "----------------------------------------------------\n";

constexpr char SIGN_IN_OK[] = "----------------------------------------------------\n\n"
                              "YOU ARE SUCCESSFULLY REGISTERED!\n\n"
                              "----------------------------------------------------\n";

constexpr char LOGIN_TRY[] = "TRY AGAIN: ";

constexpr char EMPTY_LOGIN[] = "----------------------------------------------------\n\n"
                              "SORRY, NICKNAME IS EMPTY!\n\n"
                              "----------------------------------------------------\n";
#endif //ENTERINGTASK_SERVER_CONSTEXP_H
