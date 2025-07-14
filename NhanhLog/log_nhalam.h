// log_nhalam.h
#pragma once

#define TDLOG(loai, fmt, ...) ::tdlog::log(loai, __FILE__, fmt, ##__VA_ARGS__)

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <filesystem>

namespace tdlog
{
	namespace fs = std::filesystem;

	enum class loai_log : uint8_t
	{
		thong_bao,
		canh_bao,
		loi
	};

	enum class logopt : uint8_t
	{
		none = 0,
		with_timestamp = 1 << 0,
		no_timestamp = 1 << 1
	};

	constexpr logopt with_timestamp = logopt::with_timestamp;
	constexpr logopt no_timestamp = logopt::no_timestamp;
	constexpr logopt none = logopt::none;

	enum class sink_type : uint8_t
	{
		console,
		file
	};

	class KhoiDongLog
	{
	public:
		void console(logopt opt = logopt::no_timestamp);
		void file(logopt opt = logopt::with_timestamp);
		void clear();

		// muốn an toàn thì gọi trước khi kết thúc chương trình, bình thường sẽ tự động clear khi chương trình kết thúc
		// có trường hợp do chương trình tắt đột ngột nên không kịp clear, thì gọi hàm này để đảm bảo không còn sink nào tồn tại
		void shutdown();
	private:
		void clear_console_sink();
		void clear_file_sink();

		boost::shared_ptr<boost::log::sinks::sink> console_sink_;
		boost::shared_ptr<boost::log::sinks::sink> file_sink_;
	};

	extern KhoiDongLog khoidong_log;

	std::string lay_dinhdang_tg_hientai();
	void        quayvong_log(const fs::path& thumuc_log, const fs::path& log_hientai);

	template <typename... Args>
	void log(loai_log loai, std::string_view file, std::string_view fmt, Args&&... args);

	extern boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> g_logger;
} // namespace tdlog

#include "log_nhalam_impl.inl"
