#pragma once

#include "log.h"
#include "xo/string/string_type.h"
#include "xo/filesystem/path.h"
#include <fstream>

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace xo
{
	namespace log
	{
		class XO_API sink
		{
		public:
			sink( level l = info_level );
			virtual ~sink();
			virtual void send_log_message( level l, const string& msg ) = 0;
			void try_send_log_message( level l, const string& msg );
			void set_log_level( level l );
			level get_log_level();
			bool test_log_level( level l );

		protected:
			level log_level_;
		};

		class XO_API stream_sink : public sink
		{
		public:
			stream_sink( level l, std::ostream& str );
			virtual void send_log_message( level l, const string& msg );

		protected:
			std::ostream& stream_;
		};

		class XO_API console_sink : public stream_sink
		{
		public:
			console_sink( level l );
		};

		class XO_API file_sink : public stream_sink
		{
		public:
			file_sink( level l, const path& file );
			virtual void send_log_message( level l, const string& msg ) override;
			bool good();

		protected:
			std::ofstream file_stream_;
		};
	}
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
