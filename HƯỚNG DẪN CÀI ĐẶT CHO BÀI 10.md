# Hướng dẫn Bài 10 — từ file .cc đến XML

**Máy chưa cài gì:** làm theo [Cài đặt lần đầu](CAI_DAT_LAN_DAU_BAI_10.md), rồi quay lại đây.

**Đã có NS-3 và NetAnim:** làm lần lượt sáu bước dưới đây. Tất cả lệnh Linux đều nhập trong Ubuntu.

## 1. Tải bộ bài thực hành

Trên trang repository GitHub, bấm **Code → Download ZIP** rồi giải nén. Thư mục giải nén chứa `mophong_bai10.cc` và các tài liệu này.

Chỉ muốn xem hình mẫu: mở file `mophong_bai10.xml` trong thư mục giải nén bằng NetAnim. Muốn tự chạy hoặc sửa bài: tiếp tục bước 2.

## 2. Đặt file .cc vào NS-3

Mở Start → Ubuntu. Nếu đã thấy dấu nhắc dạng `tenban@TENMAY:~$`, bạn đang ở Ubuntu rồi.

Nhập:

```bash
cd ~/ns-3-dev/scratch
explorer.exe .
```

File Explorer mở thư mục `scratch` của NS-3. Copy **`mophong_bai10.cc`** từ thư mục vừa giải nén vào đây.

Nếu `explorer.exe` không chạy, mở File Explorer trên Windows, chọn Linux → Ubuntu → home → tên tài khoản Ubuntu → ns-3-dev → scratch.

Kiểm tra trong Ubuntu:

```bash
ls -l ~/ns-3-dev/scratch/mophong_bai10.cc
```

**Cần thấy:** một dòng có tên file. Nếu báo không tồn tại, kiểm tra chỗ chép và đuôi file; không để thành `.cc.txt`.

Muốn sửa code, mở chính file trong `scratch` bằng VS Code hoặc Notepad rồi Ctrl+S. Không dán mã C++ vào terminal.

## 3. Kiểm tra CMake

```bash
cmake --version
```

**Cần thấy:** thông tin phiên bản. NS-3 3.48 yêu cầu CMake từ 3.25.

Nếu chưa có, trong Ubuntu nhập:

```bash
sudo apt update
sudo apt install -y cmake build-essential
```

Nếu đã cài bản CMake riêng tại `~/tools/cmake-3.31.6-linux-x86_64/bin/cmake`, có thể dùng:

```bash
export PATH="$HOME/tools/cmake-3.31.6-linux-x86_64/bin:$PATH"
cmake --version
```

**Chỉ dùng dòng export khi file đó đã có trên máy.** Nó không tải CMake. `$HOME` là thư mục cá nhân, `$PATH` giữ các đường dẫn tìm chương trình hiện tại. Lệnh thường không in gì và chỉ có hiệu lực trong phiên terminal này.

## 4. Build: biên dịch mã nguồn

```bash
cd ~/ns-3-dev
./ns3 build -j 2
```

**Cần thấy:** quá trình kết thúc không lỗi, trở về dấu nhắc. Nếu có `error:` hoặc `FAILED`, xử lý trước khi làm bước 5.

- `./ns3`: công cụ NS-3 trong thư mục hiện tại.
- `build`: biên dịch dự án, gồm chương trình trong `scratch`.
- `-j 2`: tối đa hai tác vụ biên dịch đồng thời.

Build tạo chương trình chạy được, **chưa phải bước tạo XML mới**.

## 5. Run: chạy mô phỏng để tạo XML

Vẫn trong `~/ns-3-dev`, nhập:

```bash
./ns3 run scratch/mophong_bai10.cc
```

**Cần thấy:** lệnh kết thúc không lỗi và dấu nhắc trở lại. Bản mã này không tự in thông báo thành công, nên kiểm tra file:

```bash
ls -lh mophong_bai10.xml mophong_bai10_flowmon.xml
```

Hai file cần có thời điểm cập nhật tương ứng lần chạy vừa rồi.

| File | Dùng để làm gì? |
|---|---|
| `~/ns-3-dev/mophong_bai10.xml` | Mở trong NetAnim để xem hình |
| `~/ns-3-dev/mophong_bai10_flowmon.xml` | Đọc thống kê gói tin |

File được ghi trong `ns-3-dev` với cách chạy này, **không nằm trong `scratch`**. File `.cc` là mã nguồn, không chạy bằng cách gõ riêng đường dẫn của nó.

## 6. Mở kết quả trong NetAnim

```bash
~/netanim-bai10/build/bin/netanim
```

Trong NetAnim:

1. Chọn tab **Animator**.
2. Bấm biểu tượng **thư mục** ở góc trên bên trái.
3. Đi tới home → tài khoản Ubuntu → ns-3-dev.
4. Chọn **`mophong_bai10.xml`**, bấm Open.
5. Khi thấy `Parsing complete: Click Play`, bấm tam giác xanh Play.

Để lấy đường dẫn đầy đủ nếu cần, nhập trong một cửa sổ Ubuntu khác:

```bash
realpath ~/ns-3-dev/mophong_bai10.xml
```

Copy kết quả vào ô File name của NetAnim. Ví dụ tài khoản Ubuntu là `user` thì đường dẫn là `/home/user/ns-3-dev/mophong_bai10.xml`; tài khoản khác có đường dẫn khác.

## Đọc hình và số liệu

Bản gốc có một UE và một trạm gốc. Với đúng mã này: node 4 là UE, 3 là eNodeB, 1 là S-GW, 0 là P-GW, 2 là MME. Đây là năm thành phần, không phải năm điện thoại. Các số trên lưới là tọa độ. Vị trí mạng lõi dùng để trình bày sơ đồ.

Mã chỉ thiết lập kết nối LTE/EPC, chưa có ứng dụng truyền số đo. Các mũi tên là gói được ghi nhận, không phải thiết bị chuyển động.

Đọc thống kê:

```bash
cd ~/ns-3-dev
cat mophong_bai10_flowmon.xml
```

| Trường | Ý nghĩa |
|---|---|
| `flowId` | Định danh luồng, không phải số node |
| `txPackets`, `rxPackets` | Số gói gửi, nhận |
| `txBytes`, `rxBytes` | Số byte gửi, nhận |
| `delaySum` | Tổng độ trễ của các gói nhận được |
| `minDelay`, `maxDelay` | Độ trễ nhỏ nhất, lớn nhất của gói nhận được |
| `sourceAddress`, `destinationAddress` | IP gửi, nhận |

Ghép phần `FlowStats` và `Ipv4FlowClassifier` bằng cùng `flowId`. Không cộng thêm các bản ghi `FlowProbes` vào tổng gói, vì cùng một gói có thể được theo dõi ở nhiều điểm.

## Làm ba yêu cầu của đề

### A. Tăng số UE từ 1 lên 3

Trong `mophong_bai10.cc`, đổi:

```cpp
ueNodes.Create(1);
```

thành:

```cpp
ueNodes.Create(3);
```

Thay hai dòng đặt vị trí sau `AnimationInterface anim(...)` bằng:

```cpp
anim.SetConstantPosition(enbNodes.Get(0), 0.0, 0.0);
anim.SetConstantPosition(ueNodes.Get(0), 20.0, 0.0);
anim.SetConstantPosition(ueNodes.Get(1), 40.0, 0.0);
anim.SetConstantPosition(ueNodes.Get(2), 60.0, 0.0);
```

`Get(0)`, `Get(1)`, `Get(2)` là UE thứ nhất, thứ hai, thứ ba trong nhóm. Lưu rồi làm lại bước 4–6. Quan sát số UE tăng và đọc lại thống kê. Mỗi UE được đặt vị trí riêng để dễ thấy trên hình.

### B. Đổi vị trí UE

Đưa về `ueNodes.Create(1);`. Thay toàn bộ đoạn đặt vị trí bằng:

```cpp
anim.SetConstantPosition(enbNodes.Get(0), 0.0, 0.0);
anim.SetConstantPosition(ueNodes.Get(0), 100.0, 0.0);
```

Bỏ các dòng `Get(1)` và `Get(2)` vì chỉ còn một UE. UE cách trạm 100 m thay vì 20 m. Lưu rồi làm lại bước 4–6. Đây là đổi vị trí cố định giữa các lần chạy, chưa phải cho UE di chuyển.

### C. Đổi thời gian mô phỏng

Đưa vị trí UE về `(20.0, 0.0)` nếu muốn chỉ thay đổi thời gian so với bản gốc. Có thể dùng tham số có sẵn:

```bash
cd ~/ns-3-dev
./ns3 run "scratch/mophong_bai10 --simTime=5s"
```

Hoặc sửa `Time simTime = MilliSeconds(1050);` thành `Time simTime = Seconds(5);`, lưu và build/run lại.

Thời gian mặc định là 1,05 giây mô phỏng. Tăng lên 5 giây không tự tạo thêm lưu lượng ứng dụng; các số gói báo hiệu có thể không đổi sau khi kết nối đã được thiết lập.

### Giữ kết quả để so sánh

Mỗi lần chạy sẽ ghi đè hai XML cùng tên. Trước khi chạy trường hợp tiếp theo, copy kết quả vào thư mục riêng, ví dụ:

```bash
cd ~/ns-3-dev
mkdir -p ket-qua/ue3
cp mophong_bai10.xml mophong_bai10_flowmon.xml ket-qua/ue3/
```

Thay `ue3` bằng `goc`, `xa100m` hoặc `thoigian5s` tùy trường hợp. Ghi lại số UE, tọa độ và thời gian của từng lần; không giả định tăng khoảng cách luôn làm tăng độ trễ báo hiệu mạng lõi.

## Lỗi thường gặp

| Hiện tượng | Cách kiểm tra |
|---|---|
| `./ns3: No such file or directory` | Nhập `cd ~/ns-3-dev` |
| Chạy đường dẫn `.cc` báo Permission denied | Dùng `./ns3 run scratch/mophong_bai10.cc`; không cần cấp quyền thực thi cho `.cc` |
| Chưa có XML sau build | Làm bước 5 — run |
| Sửa mã nhưng hình không đổi | Ctrl+S đúng bản trong scratch, run lại và mở lại XML |
| Không tìm được CMake | Xem bước 3 |
| NetAnim không đọc file | Chọn XML hoạt họa, không chọn file `_flowmon.xml` |

*Bản chia sẻ dùng mã mô phỏng cơ bản, đã build/run trên NS-3 3.48. Các bài chỉnh sửa là hướng dẫn để người học tự thực hiện.*
