/*
 * NonCanonicalModeTerminal.h
 *
 */

#ifndef NONCANONICALMODETERMINAL_H_
#define NONCANONICALMODETERMINAL_H_

#include <termios.h>
#include <unistd.h>
//#include <stdio.h>
#include <bits/stdc++.h>

struct termios saved_attributes;

void reset_input_mode (void)
{
	tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode (void)
{
	struct termios tattr;

	/* Make sure stdin is a terminal. */
	if (!isatty (STDIN_FILENO))
	{
	  fprintf (stderr, "Not a terminal.\n");
	  exit (EXIT_FAILURE);
	}

	/* Save the terminal attributes so we can restore them later. */
	tcgetattr (STDIN_FILENO, &saved_attributes);
	atexit (reset_input_mode);

	/* Set the funny terminal modes. */
	tcgetattr (STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}



#endif /* NONCANONICALMODETERMINAL_H_ */

