#include "log_nhalam.h"

int main()
{
	tdlog::khoidong_log.console(tdlog::logopt::with_timestamp);
	tdlog::khoidong_log.file(tdlog::logopt::no_timestamp);

	TDLOG(tdlog::loai_log::thong_bao, "Log test {}", 1);
	TDLOG(tdlog::loai_log::canh_bao, "cảnh báo: {}", 123);
	TDLOG(tdlog::loai_log::loi, "lỗi: {}", "xyz");

	tdlog::khoidong_log.shutdown();
	return 0;
}
