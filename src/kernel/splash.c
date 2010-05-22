/**
 * @file splash.c
 * @brief The splash scree
 */

#include "kinout.h"
#include "splash.h"

/**
 * @brief Print the splash screen
 * @param void
 * @return void
 */
void
splash(void)
{
  kprintln("\n\nWelcome in");
  kprintln(" _______ _______ _______ __  __");
  kprintln("|     __|     __|_     _|  |/  |");
  kprintln("|__     |__     |_|   |_|     < ");
  kprintln("|_______|_______|_______|__|\\__|");
  kprintln("The Simply and Stupidly Implemented Kernel");
  kprintln("Proudly procrastinated by Adrien, Chris & Mayeu");

  kprintln("\nFirst time here? Type help!");
}
