/**
 * @file splash.c
 * @brief The splash scree
 */

#include <stdio.h>
#include "splash.h"

/**
 * @brief Print the splash screen
 * @param void
 * @return void
 */
void
splash(void)
{
  print("\n\nWelcome in\n");
  print(" _______ _______ _______ __  __\n");
  print("|     __|     __|_     _|  |/  |\n");
  print("|__     |__     |_|   |_|     < \n");
  print("|_______|_______|_______|__|\\__|\n");
  print("The Simply and Stupidly Implemented Kernel\n");
  //print("Proudly procrastinated by Adrien, Chris & Mayeu\n");

  print("\nFirst time here? Type help!\n");
}
