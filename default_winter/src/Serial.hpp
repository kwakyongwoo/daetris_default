#ifndef LIB_DAETRIS_SERIAL_HPP
#define LIB_DAETRIS_SERIAL_HPP

#include "ofMain.h"

#include <functional>

namespace LibDaetris
{
	class SerialApp : public ofBaseApp {
	public:
		using callback_t = std::function<void(int)>;

		SerialApp(std::string port_name, int baud, callback_t callback = empty_callback) :
			m_available(false),
			m_port_name(port_name),
			m_baud(baud),
			m_callback(callback)
		{
			// Do Nothing
		}

		void setup() {
			m_serial.setup(m_port_name, m_baud);
			m_available = m_serial.isInitialized();
		}

		void update() {
			if (m_available && m_serial.available() > 0) {
				m_callback(m_serial.readByte());
			}
		}

		std::string const& port_name() const {
			return m_port_name;
		}

		int baud() const {
			return m_baud;
		}

		void set_callback(callback_t callback) {
			m_callback = callback;
		}

		bool available() const {
			return m_available;
		}

	private:
		bool m_available;

		std::string m_port_name;
		int m_baud;

		callback_t m_callback;
		ofSerial m_serial;

		static void empty_callback(int) {
			return;
		}
	};
}

#endif