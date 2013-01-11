#ifndef ATHENA_IO_INPUTDEVICE_HPP
#define ATHENA_IO_INPUTDEVICE_HPP



namespace athena
{

	namespace io
	{

		class InputDevice
		{
			public:

				// The instructor of the class.
				InputDevice();
				// The destructor of the class.
				virtual ~InputDevice();


				// Function responsible of performing any setup operations.
				virtual void startup();
				// Function responsible of reforming any cleanup operations.
				virtual void terminate();
				// Function responsible of performing update operations.
				virtual void update();
		};

	} /* io */

} /* athena */



#endif /* ATHENA_IO_INPUTDEVICE_HPP */