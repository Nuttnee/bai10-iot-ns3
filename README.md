# Bài 10 — Mô phỏng mạng LTE bằng NS-3

Bộ mã và hướng dẫn tiếng Việt dành cho người mới: cài môi trường, chạy một mạng LTE cơ bản và xem kết quả bằng NetAnim.

## Bắt đầu ở đâu?

1. **Máy chưa cài phần mềm:** đọc [Cài đặt lần đầu](CAI_DAT_LAN_DAU_BAI_10.md).
2. **Đã có NS-3 và NetAnim:** làm theo [HƯỚNG DẪN CÀI ĐẶT CHO BÀI 10.md](HƯỚNG%20DẪN%20CÀI%20ĐẶT%20CHO%20BÀI%2010.md).
3. **Chỉ muốn xem kết quả mẫu:** tải repository bằng **Code → Download ZIP**, giải nén và mở `mophong_bai10.xml` bằng NetAnim.

## Các file

| File | Vai trò |
|---|---|
| [mophong_bai10.cc](mophong_bai10.cc) | Mã C++ của bài mô phỏng |
| [mophong_bai10.xml](mophong_bai10.xml) | Kết quả mẫu để mở trong NetAnim |
| [mophong_bai10_flowmon.xml](mophong_bai10_flowmon.xml) | Kết quả mẫu thống kê gói tin |
| [Cài đặt lần đầu](CAI_DAT_LAN_DAU_BAI_10.md) | Cài WSL, Ubuntu, CMake, NS-3, Qt và NetAnim |
| [Hướng dẫn thực hành](HƯỚNG%20DẪN%20CÀI%20ĐẶT%20CHO%20BÀI%2010.md) | Từ file `.cc` đến XML; sửa số UE, vị trí và thời gian |

## Bài này biểu diễn điều gì?

Một UE (thiết bị) kết nối với một trạm gốc eNodeB và mạng lõi EPC. Mã mẫu thực hiện thiết lập kết nối; chưa thêm máy chủ ứng dụng hoặc lưu lượng cảm biến. Các luồng IP ghi được là báo hiệu trong mạng lõi. Không dùng độ trễ đó để kết luận độ trễ từ thiết bị qua LTE tới máy chủ Internet.

Bài dùng mô hình LTE/EPC của NS-3; không phải mô phỏng đầy đủ NB-IoT. Không cần ESP32 hoặc Arduino thật.

## Chạy nhanh khi đã cài môi trường

Đặt `mophong_bai10.cc` vào `~/ns-3-dev/scratch/`. Sau đó nhập **trong Ubuntu**:

```bash
cd ~/ns-3-dev
./ns3 build -j 2
```

Build thành công thì chạy:

```bash
./ns3 run scratch/mophong_bai10.cc
```

Kết quả mới nằm trong `~/ns-3-dev`:

- `mophong_bai10.xml`: mở bằng NetAnim.
- `mophong_bai10_flowmon.xml`: đọc bằng trình soạn thảo để xem số liệu.

**Build tạo chương trình chạy được. Run mới chạy mô phỏng và ghi XML.** Không nhập đường dẫn `.cc` như một lệnh thực thi.

## Môi trường và kết quả mẫu

Đã biên dịch và chạy bản mã này trên NS-3 3.48, Ubuntu qua WSL2, CMake 3.31.6. Hướng dẫn cài dùng NetAnim 3.110. Quy trình cài từ máy Windows trắng chưa được kiểm thử lại toàn bộ.

Kết quả mẫu: một UE, một eNodeB, thời gian mô phỏng 1,05 giây; tổng năm node gồm cả EPC. FlowMonitor ghi bốn luồng, mỗi luồng gửi/nhận một gói. Kết quả riêng sau khi sửa mã cần được đọc lại, không lấy số mẫu làm kết quả mặc định.

## Nguồn mã

Mã dựa trên mẫu LTE trong Bài thực hành 10, giữ thông tin tác giả gốc CTTC / Manuel Requena và SPDX `GPL-2.0-only`. Bản cơ bản ở đây đổi tên hai file kết quả sang `mophong_bai10.xml` và `mophong_bai10_flowmon.xml`. Xem [LICENSE](LICENSE) cho giấy phép mã nguồn.
