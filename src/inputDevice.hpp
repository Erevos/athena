#ifndef ATHENA_IO_INPUTDEVICE_HPP
#define ATHENA_IO_INPUTDEVICE_HPP



namespace athena
{

	namespace io
	{

		/*
			An enumeration holding all the possible input devices.
		*/
		enum DeviceType
		{
			IsKeyboard = 0,
			IsMouse
		};


		/*
			A class representing and handling a input device.
		*/
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


				// Function returning the type of the device.
				virtual DeviceType type() const = 0;
		};

	} /* io */

} /* athena */



#endif /* ATHENA_IO_INPUTDEVICE_HPP */