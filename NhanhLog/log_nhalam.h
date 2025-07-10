// log_nhalam.h
#pragma once

#define TDLOG(loai, fmt, ...) ::tdlog::log(loai, __FILE__, fmt, ##__VA_ARGS__)

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <filesystem>

namespace tdlog
{
	namespace fs = std::filesystem;

	enum log_flag : uint32_t
	{
		None       = 0,      // mặc định, không cần dùng cái này chỉ cần xóa hết cờ là được
		ToFile     = 1 << 0, // log ra file
		ToConsole  = 1 << 1, // log ra console
		AutoRotate = 1 << 2, // bật quay vòng file, không gọi với cờ console vì nó không sẽ không hoạt động
	};

	inline log_flag operator|(log_flag a, log_flag b)
	{
		return static_cast<log_flag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}

	inline bool operator&(log_flag a, log_flag b)
	{
		return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
	}

	inline bool g_enable_log = true;

	enum class loai_log : uint8_t
	{
		thong_bao,
		canh_bao,
		loi
	};

	std::string lay_dinhdang_tg_hientai();
	void        quayvong_log(const fs::path& thumuc_log, const fs::path& log_hientai);

	void khoitao_boostlog(log_flag flags);

	template <typename... Args>
	void log(loai_log loai, std::string_view file, std::string_view fmt, Args&&... args);

	extern boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> g_logger;
} // namespace tdlog

#include "log_nhalam_impl.inl"
