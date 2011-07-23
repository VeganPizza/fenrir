#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <SDL/SDL.h>

struct point
{
	int input;
	int output;
};

/* These maps are a set of points defining the joystick input / arduino output mapping function.
   Linear interpolation is done for points not explicitly specified.
   These points must be sorted by input, and duplicate inputs are not allowed.
 */
static point left_map[] =  { {-32768, 250}, {-31744, 250}, {-20000, 210}, { -1024, 190},
	{  1024, 190}, {20000,170}, { 31744, 130}, { 32767, 130} };

static point right_map[] = { {-32768, 250}, {-31744, 250}, {-20000, 210}, { -1024, 190},
	{1024, 190}, {20000, 170}, { 31744, 130}, { 32767, 130} };

enum buttons { B1, B2, B3, B4, L1, R1, L2, R2, B_SIZE }; //controller buttons 
enum sticks { LEFTHORZ, LEFTVERT, RIGHTHORZ, RIGHTVERT, S_SIZE }; //controller joysticks
enum inputs { LBIT, RBIT, B1BIT, B2BIT, B3BIT, B4BIT, L1BIT, R1BIT, L2BIT, R2BIT, I_SIZE  }; //array locations for out/input

int input_buf[] = {};

static int open_serial_port (const char * port)
{
	int fd = open(port, O_RDWR, 0);
	if (fd < 0)
	{
		printf("Unable to open serial port %s: ", port);
		perror("");
		return -1;
	}    
	return fd;
}

static SDL_Joystick * open_joystick ()
{
	if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_Init: %s", SDL_GetError());
		return NULL;
	}

	int num_joysticks = SDL_NumJoysticks();
	if (num_joysticks == 0)
	{
		printf("No joysticks found\n");
		return NULL;
	}
	printf("%d joystick(s) found\n", num_joysticks);
	printf("Using joystick \"%s\"\n", SDL_JoystickName(0));

	SDL_Joystick * joystick = SDL_JoystickOpen(0);
	if (!joystick)
	{
		printf("SDL_JoystickOpen: %s", SDL_GetError());
		return NULL;
	}

	SDL_JoystickEventState(SDL_ENABLE);
	return joystick;
}

static int apply_map (int input, const point points[], int num_points)
{
	/* Do a binary search to find the 2-point interval */
	int start = 0;
	int end = num_points - 1;
	while (start + 1 != end)
	{
		int index = (start + end) / 2;
		if (input < points[index].input)
		{
			end = index;
		}
		else
		{
			start = index;
		}
	}
	/* Do linear interpolation to determine output */
	return points[start].output + (points[end].output - points[start].output) * (input - points[start].input)
		/ (points[end].input - points[start].input);
};

static int get_input (int input[I_SIZE]) //used to be 2 Mar 1
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_JOYAXISMOTION:
				if (event.jaxis.axis == LEFTVERT) 
				{
					input[LBIT] = event.jaxis.value;
				}

				if (event.jaxis.axis == RIGHTVERT) 
				{
					input[RBIT] = event.jaxis.value;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				if(event.jbutton.button == B1) //1
				{
					input[B1BIT] = 1;
				}
				if(event.jbutton.button == B2) //2
				{
					input[B2BIT] = 1;
				}
				if(event.jbutton.button == B3) //3
				{
					input[B3BIT] = 1;
				}
				if(event.jbutton.button == B4) //4
				{
					input[B4BIT] = 1;
				}
				if(event.jbutton.button == L1) //L1
				{
					input[L1BIT] = 1;
				}
				if(event.jbutton.button == R1) //R1
				{
					input[R1BIT] = 1;
				}
				if(event.jbutton.button == L2) //L2
				{
					input[L2BIT] = 1;
				}
				if(event.jbutton.button == R2) //R2
				{
					input[R2BIT] = 1;
				}
							
				break;
			case SDL_JOYBUTTONUP:
				if(event.jbutton.button == B1)
				{
					input[B1BIT] = 0;
				}
				if(event.jbutton.button == B2)
				{
					input[B2BIT] = 0;
				}
				if(event.jbutton.button == B3)
				{
					input[B3BIT] = 0;
				}
				if(event.jbutton.button == B4)
				{	
					input[B4BIT] = 0;
				}
				if(event.jbutton.button == L1)
				{
					input[L1BIT] = 0;
				}
				if(event.jbutton.button == R1)
				{
					input[R1BIT] = 0;
				}
				if(event.jbutton.button == L2)
				{
					input[L2BIT] = 0;
				}
				if(event.jbutton.button == R2)
				{	
					input[R2BIT] = 0;
				}
				break;
			case SDL_QUIT:
				exit(EXIT_SUCCESS);
				break;
		}
	}
	return 0;
}

static int send_output (int port_fd, const int output[I_SIZE])
{
	char output_buf[] = { output[LBIT], output[RBIT], output[B1BIT], output[B2BIT], output[B3BIT], output[B4BIT], output[L1BIT], output[R1BIT], output[L2BIT], output[R2BIT], 10 };               
	if (write(port_fd, output_buf, sizeof(output_buf)) < 0)
	{
		perror("write");
		return -1;
	}
	return 0;
}

int main (int argc, char * argv[])
{
	const char * port = NULL;
	if (argc < 2)
	{
		printf("No serial port given; not outputting to serial\n");
	}
	else
	{
		port = argv[1];
	}

	int port_fd;
	if (port)
	{
		port_fd = open_serial_port(port);
		if (port_fd < 0)
		{
			return EXIT_FAILURE;
		}
	}

	SDL_Joystick * joystick = open_joystick();
	if (!joystick)
	{
		return EXIT_FAILURE;
	}

	int input[I_SIZE] = {};
	int output[I_SIZE] = {};

	while (true)
	{
		get_input(input);
	
		output[RBIT] = apply_map(input[LBIT], left_map, sizeof(left_map) / sizeof(left_map[0]));
		output[LBIT] = apply_map(input[RBIT], right_map, sizeof(right_map) / sizeof(right_map[0]));
		output[B1BIT] = input[B1BIT];
		output[B2BIT] = input[B2BIT]; 
		output[B3BIT] = input[B3BIT];
		output[B4BIT] = input[B4BIT];
		output[L1BIT] = input[L1BIT];
		output[R1BIT] = input[R1BIT];
		output[L2BIT] = input[L2BIT];
		output[R2BIT] = input[R2BIT];



		if (port)
		{
			send_output(port_fd, output);
		}
		
		printf("%7d %7d %1d %1d %1d %1d %1d %1d %1d %1d | %7d %7d %1d %1d %1d %1d %1d %1d %1d %1d\r", input[LBIT], input[RBIT], input[B1BIT],input[B2BIT], input[B3BIT], input[B4BIT], input[L1BIT], input[R1BIT], input[L2BIT], input[R2BIT], output[LBIT], output[RBIT], output[B1BIT], output[B2BIT], output[B3BIT], output[B4BIT], output[L1BIT], output[R1BIT], output[L2BIT], output[R2BIT]);

		fflush(stdout);
		usleep(10000);
	}
}

