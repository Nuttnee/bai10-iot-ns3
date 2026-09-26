# Bài 10 — Cài đặt lần đầu trên máy Windows

**Chỉ dùng tài liệu này khi máy chưa có môi trường thực hành.** Nếu máy đã build NS-3, chạy bài và mở NetAnim thành công thì quay lại [hướng dẫn từ .cc đến XML](HƯỚNG%20DẪN%20CÀI%20ĐẶT%20CHO%20BÀI%2010.md).

Làm theo thứ tự **1 → 2 → 3 → 4 → 5**. Bước nào báo lỗi thì dừng ở bước đó.

## 1. Chuẩn bị máy

- Windows 11 hoặc Windows 10 từ phiên bản 2004, build 19041 để dùng cách cài WSL dưới đây.
- Có Internet và quyền quản trị Windows để cài WSL.
- Nên có RAM 8 GB trở lên, khoảng 15–20 GB dung lượng trống dự phòng.
- Có bộ mã B10 IOT để thực hành sau khi cài.

Bài này mô phỏng trên máy tính, không cần ESP32 hoặc Arduino thật.

Bạn sẽ cài:

| Thành phần | Công dụng |
|---|---|
| WSL + Ubuntu | Chạy môi trường Linux bên trong Windows |
| Bộ công cụ C++, CMake, Python, Git | Tải mã và biên dịch chương trình |
| NS-3 | Chạy mô phỏng mạng |
| Qt + NetAnim | Tạo và mở ứng dụng xem kết quả mô phỏng |

## 2. Cài WSL và mở Ubuntu

### 2a. Kiểm tra máy đã có Ubuntu chưa

**Thao tác trên Windows:** Start → gõ `PowerShell` → mở ứng dụng.

**Nhập trong PowerShell:**

```powershell
wsl --list --verbose
```

Nếu đã có Ubuntu với VERSION bằng **2**, không cài lại: chuyển đến bước 2c. Nếu chưa có WSL/Ubuntu, làm bước 2b.

### 2b. Cài lần đầu

Đóng cửa sổ PowerShell thường. Start → gõ `PowerShell` → nhấp chuột phải → **Run as administrator** → Yes.

**Nhập trong PowerShell quản trị:**

```powershell
wsl --install -d Ubuntu
```

Chờ cài xong, lưu công việc và khởi động lại Windows nếu được yêu cầu.

Nguồn: [Microsoft — cài WSL](https://learn.microsoft.com/en-us/windows/wsl/install).

### 2c. Mở Ubuntu

Start → gõ **Ubuntu** → mở ứng dụng.

Lần mở đầu, nếu được hỏi:

1. Đặt tên tài khoản Ubuntu, ví dụ `sinhvien`.
2. Đặt mật khẩu và nhập lại mật khẩu đó.
3. Khi nhập mật khẩu Linux, không hiện chữ hoặc dấu sao; gõ xong rồi Enter.

**Cần thấy:** dấu nhắc dạng `sinhvien@TENMAY:~$` hoặc `user@N:~$`.

**Từ đây, mọi lệnh ở bước 3–5 đều nhập trong Ubuntu.** Không nhập `wsl -d Ubuntu` trong cửa sổ này; bạn đã vào Ubuntu rồi.

Nếu không tìm thấy ứng dụng Ubuntu, mở PowerShell và nhập `wsl -d Ubuntu`. Nếu tên bản phân phối khác, dùng đúng tên trong kết quả `wsl --list --verbose`.

Nếu Ubuntu đang dùng WSL1, chuyển bằng `wsl --set-version Ubuntu 2` trong PowerShell. Thay `Ubuntu` bằng đúng tên bản phân phối nếu khác.

<a id="cai-cong-cu"></a>

## 3. Cài các công cụ

**Trong Ubuntu, nhập lệnh đầu và chờ xong:**

```bash
sudo apt update
```

Lệnh cập nhật danh sách phần mềm. Nếu hỏi mật khẩu, nhập mật khẩu Ubuntu đã đặt ở bước 2.

**Sau đó nhập:**

```bash
sudo apt install -y build-essential python3 git cmake ninja-build pkg-config qtbase5-dev
```

Lệnh cài trình biên dịch C++, Python, Git, CMake và thư viện Qt cho NetAnim.

**Kiểm tra khi cài xong:**

```bash
g++ --version
python3 --version
git --version
cmake --version
```

**Cần thấy:** cả bốn lệnh in phiên bản, không báo `command not found`.

Bộ NS-3 3.48 dùng trong hướng dẫn yêu cầu CMake từ 3.25 và GCC từ 11 theo cấu hình nguồn của phiên bản này. Nếu công cụ quá cũ, cần cập nhật công cụ hoặc Ubuntu trước khi sang bước 4.

### Nếu chỉ thiếu CMake

**Trong Ubuntu:**

```bash
sudo apt update
sudo apt install -y cmake build-essential
cmake --version
```

CMake cài bằng cách này thường gọi trực tiếp được, không cần tự thêm PATH. Bộ CMake cho Windows không thay thế CMake trong Ubuntu của bài này.

### Nếu dùng CMake riêng như máy đang học

Chỉ dùng cách dưới khi file `~/tools/cmake-3.31.6-linux-x86_64/bin/cmake` đã có thật:

```bash
export PATH="$HOME/tools/cmake-3.31.6-linux-x86_64/bin:$PATH"
cmake --version
```

`export` giúp Ubuntu tìm CMake trong phiên terminal này. Nó không tải hay cài CMake, không tự tạo thư mục trên máy mới. Mở terminal mới có thể cần nhập lại.

### Nếu xuất hiện `Waiting for cache lock`

Ubuntu có thể đang tự cập nhật. Chờ tiến trình cài đặt kia hoàn tất, không xóa file khóa. Dòng `packages can be upgraded` sau `apt update` là thông báo có bản nâng cấp, không phải lỗi.

## 4. Tải và build NS-3

Hướng dẫn cố định **NS-3 3.48** cho bộ mã này; không yêu cầu tìm bản mới nhất.

### 4a. Tải mã nguồn

**Trong Ubuntu:**

```bash
cd ~
git clone --depth 1 --branch ns-3.48 https://gitlab.com/nsnam/ns-3-dev.git ns-3-dev
```

**Cần thấy:** clone kết thúc không lỗi, có thư mục `~/ns-3-dev`. Thông báo `detached HEAD` khi chọn một phiên bản cố định không phải lỗi.

Nếu báo thư mục đã tồn tại, đừng xóa để tải lại. Kiểm tra đó có phải bộ NS-3 đang dùng hay không trước khi tiếp tục.

### 4b. Chuẩn bị cấu hình

**Trong Ubuntu:**

```bash
cd ~/ns-3-dev
./ns3 configure --build-profile=optimized --disable-examples --disable-tests --disable-python --disable-werror --enable-modules=lte,flow-monitor,netanim,applications,point-to-point,internet,mobility,csma,config-store
```

`configure` chọn các phần cần cho bài và chuẩn bị quá trình biên dịch.

**Cần thấy:** `Configuring done`, `Generating done`, không có lỗi kết thúc.

### 4c. Biên dịch NS-3

```bash
./ns3 build -j 2
```

Chờ hoàn thành, lần đầu có thể mất nhiều phút. `-j 2` cho phép tối đa hai tác vụ biên dịch cùng lúc.

**Cần thấy:** kết thúc không lỗi và dấu nhắc Ubuntu xuất hiện lại.

**Đến đây đã có NS-3, chưa có kết quả XML của bài.** Muốn có XML, cần thêm mã bài rồi chạy theo hướng dẫn thực hành.

Tham khảo: [tài liệu cài NS-3](https://www.nsnam.org/docs/installation/singlehtml/), [mã nguồn phiên bản 3.48](https://gitlab.com/nsnam/ns-3-dev/-/tree/ns-3.48).

## 5. Cài NetAnim

Qt đã được cài bằng `qtbase5-dev` ở bước 3. Không cần cài Qt Creator để thực hiện quy trình này.

### 5a. Tải NetAnim

**Trong Ubuntu:**

```bash
cd ~
git clone --depth 1 --branch netanim-3.110 https://gitlab.com/nsnam/netanim.git netanim-bai10
```

Chỉ clone khi chưa có thư mục `~/netanim-bai10`. Hướng dẫn dùng [NetAnim 3.110](https://gitlab.com/nsnam/netanim/-/tree/netanim-3.110).

### 5b. Chuẩn bị cấu hình

```bash
cmake -S ~/netanim-bai10 -B ~/netanim-bai10/build
```

`-S` chỉ thư mục mã nguồn; `-B` chỉ nơi chứa kết quả build.

**Cần thấy:** `Configuring done` và `Generating done`, không có lỗi kết thúc.

### 5c. Biên dịch NetAnim

```bash
cmake --build ~/netanim-bai10/build -j 2
```

**Cần thấy:** `[100%] Built target netanim`.

### 5d. Mở thử

```bash
~/netanim-bai10/build/bin/netanim
```

**Cần thấy:** cửa sổ NetAnim với tab Animator, Stats, Packets. Màn hình trắng yêu cầu chọn XML là bình thường vì chưa nạp kết quả bài.

Nếu ứng dụng báo không kết nối được màn hình, cần kiểm tra hỗ trợ ứng dụng đồ họa WSL trên Windows đang dùng. Không phải mọi lỗi mở giao diện đều do biên dịch thất bại.

**Cài đặt xong.** Quay lại [Bước 1 — Từ file .cc đến XML](HƯỚNG%20DẪN%20CÀI%20ĐẶT%20CHO%20BÀI%2010.md) để chép mã, build bài, chạy mô phỏng và mở XML.

## Phân biệt cảnh báo với lỗi dừng

- `CMake Error`, `FAILED`, lỗi compiler: xử lý trước khi sang bước sau.
- Cảnh báo đường dẫn Windows trong WSL: nếu configure/build vẫn hoàn tất thành công thì không tự có nghĩa bài bị lỗi. Chưa cần sửa `/etc/wsl.conf` chỉ để làm tiếp hướng dẫn này.
- Thiếu Doxygen, Sphinx hoặc thư viện tùy chọn: xem trạng thái kết thúc và module cần cho bài; không cần cài toàn bộ danh sách chỉ để xóa mọi thông báo.
- Thay đổi `appendWindowsPath` có thể ảnh hưởng việc gọi `explorer.exe` từ Ubuntu. Nếu lệnh đó không hoạt động, dùng đường dẫn `\\wsl.localhost\...` trong File Explorer như hướng dẫn thực hành.

*Cập nhật 26/09/2026. Các bước được biên soạn từ hướng dẫn trước và môi trường đã sử dụng; chưa kiểm thử lại toàn bộ trên một máy Windows trắng.*

