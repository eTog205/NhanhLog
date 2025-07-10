#include "log_nhalam.h"

#include <boost/log/expressions.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <windows.h>

namespace tdlog
{
	using namespace std::chrono;
	using async_file_sink = boost::log::sinks::asynchronous_sink<boost::log::sinks::text_file_backend>;

	boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> g_logger;

	std::string lay_dinhdang_tg_hientai()
	{
		const auto now       = system_clock::now();
		auto       in_time_t = system_clock::to_time_t(now);
		std::tm    local_tm{};
		localtime_s(&local_tm, &in_time_t);

		char buf[20];
		std::strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &local_tm);
		return buf;
	}

	void quayvong_log(const fs::path& thumuc_log, const fs::path& log_hientai)
	{
		if (fs::exists(log_hientai))
		{
			const auto     timestamp = lay_dinhdang_tg_hientai();
			const fs::path new_file  = thumuc_log / (timestamp + ".txt");
			try
			{
				fs::rename(log_hientai, new_file);
			}
			catch (const fs::filesystem_error& e)
			{
				MessageBoxA(nullptr, e.what(), "Log lỗi: không thể hoàn thành quay vòng", MB_OK | MB_ICONERROR);
			}
		}
	}

	void khoitao_boostlog(log_flag flags)
	{
		if (!g_enable_log)
			return;

		boost::log::core::get()->remove_all_sinks();
		boost::log::add_common_attributes();
		g_logger.add_attribute("TimeStamp", boost::log::attributes::local_clock());

		if ((flags & ToFile))
		{
			fs::create_directories("log");
			const fs::path logDir     = "log";
			const fs::path currentLog = logDir / "hientai.txt";

			if (flags & AutoRotate)
				quayvong_log(logDir, currentLog);

			// File log luôn async
			using async_file_sink = boost::log::sinks::asynchronous_sink<boost::log::sinks::text_file_backend>;
			auto backend = boost::make_shared<boost::log::sinks::text_file_backend>(boost::log::keywords::file_name =
			                                                                            currentLog.string());
			auto sink    = boost::make_shared<async_file_sink>(backend);
			sink->set_formatter(
			    boost::log::expressions::stream
			    << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
			    << " " << boost::log::expressions::smessage);
			sink->locked_backend()->auto_flush(true);
			boost::log::core::get()->add_sink(sink);
		}

		if ((flags & ToConsole))
		{
			boost::log::add_console_log(
			    std::clog,
			    boost::log::keywords::format =
			        (boost::log::expressions::stream
			         << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S")
			         << " " << boost::log::expressions::smessage));
		}
	}

} // namespace tdlog
