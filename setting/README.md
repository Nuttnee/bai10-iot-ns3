# Bản setting — 3 UE

Bản mã và kết quả do người học chạy, giữ nguyên tại thời điểm tải lên.

- `mophong_bai10_setting.cc`: mã nguồn.
- `mophong_bai10_setting.xml`: mở bằng NetAnim.
- `mophong_bai10_flowmon.xml`: thống kê của lần chạy setting; đặt riêng để không ghi đè kết quả bản gốc.

## Điểm cần sửa khi thực hành

Mã tạo 3 UE nhưng cả ba dòng đặt vị trí đều dùng `ueNodes.Get(0)`. Chỉ UE thứ nhất được đặt lại lần lượt ở 20, 40, 60 m; hai UE còn lại chưa được đặt vị trí riêng. Muốn ba UE ở 20, 40, 60 m, sửa lần lượt thành `Get(0)`, `Get(1)`, `Get(2)`, lưu rồi chạy lại. XML đi kèm là kết quả trước khi sửa điểm này.

Thời gian mặc định là 1050 ms (1,05 giây). Mã vẫn xuất thống kê với tên `mophong_bai10_flowmon.xml`; chạy cùng thư mục với bản gốc sẽ ghi lại file thống kê cùng tên.
