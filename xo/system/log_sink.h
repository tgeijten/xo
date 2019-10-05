#pragma once

#include "log.h"
#include "xo/string/string_type.h"
#include "xo/filesystem/path.h"
#include <fstream>

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
			virtual void flush() {}
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
			virtual void send_log_message( level l, const string& msg ) override;
			virtual void flush() override;

		protected:
			std::ostream& stream_;
		};

		class XO_API console_sink : public stream_sink
		{
		public:
			console_sink( level l );
			virtual void send_log_message( level l, const string& msg ) override;
		};

		class XO_API file_sink : public stream_sink
		{
		public:
			file_sink( level l, const path& file );
			virtual void send_log_message( level l, const string& msg ) override;
			const std::ofstream& file_stream() const { return file_stream_; }

		protected:
			std::ofstream file_stream_;
		};
	}
}
