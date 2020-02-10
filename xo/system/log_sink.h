#pragma once

#include "log.h"
#include "xo/string/string_type.h"
#include "xo/filesystem/path.h"

#include <fstream>
#include <thread>

namespace xo
{
	namespace log
	{
		enum class sink_mode { all_threads, current_thread };

		class XO_API sink
		{
		public:
			sink( level l, sink_mode m = sink_mode::all_threads );
			virtual ~sink();

			/// returns true if log_level is accepted
			virtual bool test_log_level( level l ) const;

			/// calls hande_log_message( l, msg ) if test_log_level( l ) == true
			virtual void submit_log_message( level l, const string& msg );

			/// flush log messages
			virtual void flush() {}

			/// set / get internal log level
			virtual void set_log_level( level l );
			virtual level get_log_level();

			/// set mode, updates thread_id_ if sink_mode::current_thread
			void set_sink_mode( sink_mode m );

		protected:
			level log_level_;
			sink_mode sink_mode_;
			std::thread::id thread_id_;

			/// handle log message without testing
			virtual void hande_log_message( level l, const string& msg ) = 0;
		};

		class XO_API stream_sink : public sink
		{
		public:
			stream_sink( std::ostream& str, level l, sink_mode m = sink_mode::all_threads );
			virtual void hande_log_message( level l, const string& msg ) override;
			virtual void flush() override;

		protected:
			std::ostream& stream_;
		};

		class XO_API console_sink : public stream_sink
		{
		public:
			console_sink( level l, sink_mode m = sink_mode::all_threads );
			virtual void hande_log_message( level l, const string& msg ) override;
		};

		class XO_API file_sink : public stream_sink
		{
		public:
			file_sink( const path& file, level l, sink_mode m = sink_mode::all_threads );
			virtual void hande_log_message( level l, const string& msg ) override;
			const std::ofstream& file_stream() const { return file_stream_; }

		protected:
			std::ofstream file_stream_;
		};
	}
}
