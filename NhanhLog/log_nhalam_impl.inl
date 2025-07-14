// log_nhalam_impl.inl
#pragma once

namespace tdlog
{
	template <typename... Args>
	void log(loai_log loai, std::string_view tentep, std::string_view fmt, Args&&... args)
	{
		std::string      thongdiep = std::vformat(fmt, std::make_format_args(args...));
		const char* fname = tentep.data();
		const char* slash = std::max(strrchr(fname, '/'), strrchr(fname, '\\'));
		std::string_view short_file = slash ? (slash + 1) : fname;

		switch (loai)
		{
			case loai_log::thong_bao:
				BOOST_LOG_SEV(g_logger, boost::log::trivial::info)
					<< std::format("[✅ Thông báo] ({}): {}", short_file, thongdiep);
				break;
			case loai_log::canh_bao:
				BOOST_LOG_SEV(g_logger, boost::log::trivial::warning)
					<< std::format("[⚠️ Cảnh báo] ({}): {}", short_file, thongdiep);
				break;
			case loai_log::loi:
				BOOST_LOG_SEV(g_logger, boost::log::trivial::error)
					<< std::format("[❌ Lỗi] ({}): {}", short_file, thongdiep);
				break;
		}
	}
}
