# Báo cáo thực hành Bài 10 — Cài NS-3 và NetAnim trên Debian, chạy mô phỏng LTE

**Học phần:** Truyền thông trong IoT · Khoa Công nghệ Thông tin · Trường Đại học Sư phạm Kỹ thuật Hưng Yên

| Thông tin | Nội dung |
|---|---|
| Sinh viên | ......................................... |
| Mã sinh viên | ......................................... |
| Lớp | ......................................... |
| Ngày thực hiện | ......................................... |

> Cập nhật ngày 30/09/2026. **Hướng dẫn Debian ở mục 2–3; phân tích kết quả mẫu ở mục 4–8.**
> Quy trình dùng Debian 12 trong VirtualBox, PuTTY để nhập lệnh và desktop Xfce để mở NetAnim.

---

## Tra cứu nhanh

| Bạn muốn làm gì? | Mở phần này |
|---|---|
| Kết nối Debian bằng PuTTY | [3.1. SSH và PuTTY](#buoc-31) |
| Cài NS-3 từ đầu | [3.2. Công cụ](#buoc-32) → [3.3. Tải mã](#buoc-33) → [3.4. Build](#buoc-34) |
| Đã có file `.cc`, muốn tạo XML | [3.5. Chạy bài](#buoc-35) |
| Cài ứng dụng NetAnim | [3.6. Cài NetAnim](#buoc-36) |
| Lỗi `could not connect to display` | [3.7. Desktop Debian](#buoc-37) |
| Mở XML bằng NetAnim | [3.8. Mở kết quả](#buoc-38) |
| Đọc thống kê / lưu bằng chứng | [3.9. Lưu kết quả Debian](#buoc-39) |
| Tìm lỗi thường gặp | [3.10. Xử lý lỗi](#buoc-310) |
| Xem kết quả mẫu và nhận xét | [4. Kết quả](#ket-qua) · [6. Ba thay đổi](#thi-nghiem) |

> **Điểm cần nhớ:** PuTTY dùng để cài, build và chạy NS-3. Mở NetAnim trong **Terminal của desktop Debian**. Build tạo chương trình; run mới tạo XML.

---

## 1. Mục tiêu

Theo yêu cầu của đề bài:

1. Tự tạo và chạy một mô phỏng LTE tối giản trong NS-3.
2. Biết vị trí và định dạng kết quả NS-3 sinh ra.
3. Đọc được các thông số cơ bản trong tệp FlowMonitor: `txBytes`, `rxBytes`, `delay`…
4. Xem minh hoạ mô phỏng bằng NetAnim.
5. Thực hiện ba thay đổi: tăng số UE, đổi vị trí UE, đổi thời gian mô phỏng.

---

## 2. Môi trường Debian và cách đọc hướng dẫn

**Môi trường thực hành chuyển sang Debian 12 trong VirtualBox.** PuTTY dùng để điều khiển Debian từ Windows; NetAnim được mở trong desktop Debian.

| Thành phần | Vai trò / phiên bản dùng trong hướng dẫn |
|---|---|
| Windows | Máy thật, chạy VirtualBox và PuTTY |
| VirtualBox | Chạy máy ảo Debian |
| Debian 12 Bookworm | Hệ điều hành cài NS-3 và NetAnim |
| OpenSSH Server | Cho phép PuTTY kết nối vào Debian |
| NS-3 3.48 | Biên dịch và chạy mô phỏng LTE/EPC |
| g++ và CMake | Công cụ build; bản NS-3 này cần GCC từ 11, CMake từ 3.25 |
| NetAnim 3.110 và Qt | Ứng dụng xem XML hoạt họa và thư viện giao diện |
| Xfce, LightDM | Desktop Debian và màn hình đăng nhập đồ họa |

**Phân biệt nơi thao tác:**

| Nơi thao tác | Dùng cho việc gì? |
|---|---|
| Cửa sổ dòng lệnh Debian trong VirtualBox | Cài SSH lần đầu, kiểm tra khi PuTTY chưa kết nối được |
| PuTTY trên Windows, đã đăng nhập Debian | Cài phần mềm, tải mã, build, chạy mô phỏng, đọc thống kê |
| Terminal trong desktop Xfce của Debian | Mở cửa sổ NetAnim |
| VirtualBox Manager trên Windows | Cấu hình mạng và bật/tắt máy ảo |

PuTTY và cửa sổ Debian truy cập **cùng máy ảo và cùng file** khi đăng nhập cùng tài khoản. Không cần cài một NS-3 riêng cho PuTTY. PuTTY thông thường chỉ hiển thị văn bản, không tự hiển thị cửa sổ NetAnim.

**Kiểm tra trực tiếp qua SSH ngày 30/09/2026:** Debian 12 có NS-3 3.48, CMake 3.25.1, executable `~/netanim-bai10/build/bin/netanim`, gói `task-xfce-desktop` và `lightdm`; dịch vụ LightDM đang `active`. Lỗi `could not connect to display` trước đó xảy ra khi mở NetAnim trong terminal không có phiên đồ họa. Trạng thái dịch vụ chưa thay thế việc mở thử NetAnim trong desktop Debian; thao tác này làm ở mục 3.8.

**Nguồn số liệu:** các XML mẫu và số liệu ở mục 4–6 thuộc lần thực hành trước trên Ubuntu/WSL2. Không đổi nhãn chúng thành kết quả đo trên Debian. Sau khi chạy lại trên Debian, lưu XML mới và điền kết quả thực tế vào báo cáo.

---

## 3. Hướng dẫn cài NS-3 và NetAnim trên Debian

### Lộ trình — chọn đúng bước đang cần

| Tình trạng hiện tại | Bắt đầu ở đâu? |
|---|---|
| Đã có Debian, chưa dùng được PuTTY | 3.1 — SSH và kết nối PuTTY |
| PuTTY đã đăng nhập Debian, chưa cài NS-3 | 3.2 — Cài công cụ |
| Đã có thư mục NS-3 | 3.3 — Kiểm tra trước khi tải lại |
| NS-3 đã build xong | 3.5 — Chạy bài để tạo XML |
| NetAnim báo `could not connect to display` | 3.7 — Cài và mở desktop |
| NetAnim đã mở được | 3.8 — Chọn XML và phát mô phỏng |

**Quy tắc:** nhập từng bước, chờ lệnh kết thúc rồi làm tiếp. Nếu báo lỗi, xử lý ngay ở bước đó. Chỉ sao chép lệnh trong ô, không chép dấu nhắc `nuthere@debian-iot:~$`. Tên `nuthere` là tài khoản của máy minh họa; người khác dùng tên đã tạo trên Debian của mình.

<a id="buoc-31"></a>

### 3.1. Kết nối Debian bằng PuTTY — chỉ thiết lập lần đầu

**Nếu đã vào được PuTTY, bỏ qua mục này và sang 3.2.**

**Nhập tại cửa sổ Debian trong VirtualBox:**

```bash
sudo apt update
sudo apt install -y openssh-server
sudo systemctl enable --now ssh
systemctl is-active ssh
hostname -I
```

Thực hiện từng dòng. `systemctl is-active ssh` cần trả về **`active`**. `enable --now` vừa khởi động SSH ngay, vừa cho nó tự khởi động cùng Debian ở các lần sau.

Mật khẩu sau `sudo` là mật khẩu Debian. Khi gõ không hiện chữ hoặc dấu sao là bình thường. Nếu gặp `sudo: command not found` hoặc tài khoản không được dùng sudo, dừng để xử lý quyền tài khoản; không chạy các bước build NS-3 dưới tài khoản root.

**Trong VirtualBox Manager trên Windows:** chọn máy Debian → Settings → Network → Adapter 1. Quy trình dưới đây dùng **Attached to: NAT**. Mở Advanced → Port Forwarding và thêm:

| Name | Protocol | Host IP | Host Port | Guest IP | Guest Port |
|---|---|---|---|---|---|
| SSH | TCP | 127.0.0.1 | 2222 | Để trống | 22 |

Bấm OK để lưu. Nếu cài đặt bị khóa, lưu công việc, tắt Debian bằng `sudo poweroff`, chỉnh cấu hình rồi Start lại máy ảo. Nếu đã dùng Bridged hoặc cách kết nối SSH khác hoạt động, không cần đổi sang NAT chỉ để làm bài.

**Trong PuTTY trên Windows:**

| Ô | Điền |
|---|---|
| Host Name | `127.0.0.1` |
| Port | `2222` |
| Connection type | SSH |
| Saved Sessions | Có thể lưu tên `Debian-Bai10` để dùng lần sau |

Bấm Open, đăng nhập bằng tài khoản và mật khẩu Debian. Lần đầu có thể xuất hiện thông báo nhận diện máy chủ SSH; kiểm tra đó là máy ảo vừa cấu hình trước khi lưu khóa.

**Dấu hiệu xong:** PuTTY hiện dấu nhắc như `nuthere@debian-iot:~$`.

Mỗi buổi chỉ cần bật máy ảo, chờ Debian khởi động, mở phiên PuTTY đã lưu rồi đăng nhập. Không cần đăng nhập trước trong cửa sổ VirtualBox. Chỉ bật Windows mà chưa bật máy ảo thì chưa kết nối được.

<a id="buoc-32"></a>

### 3.2. Cài công cụ hỗ trợ NS-3

**Nhập trong PuTTY đã đăng nhập Debian:**

```bash
sudo apt update
```

Chờ xong rồi nhập:

```bash
sudo apt install -y build-essential python3 git cmake ninja-build pkg-config ca-certificates
```

| Công cụ | Công dụng |
|---|---|
| `build-essential` | Trình biên dịch C++ và công cụ build cơ bản |
| `python3` | Chạy công cụ quản lý `./ns3` |
| `git` | Tải mã nguồn |
| `cmake`, `ninja-build` | Chuẩn bị và thực hiện build |
| `pkg-config` | Hỗ trợ tìm thư viện |
| `ca-certificates` | Chứng chỉ để kết nối HTTPS |

Kiểm tra:

```bash
cmake --version
g++ --version
python3 --version
git --version
```

**Dấu hiệu xong:** các lệnh in phiên bản, không báo `command not found`. NS-3 3.48 trong bài cần CMake từ 3.25 và GCC từ 11.

CMake cài bằng `apt` được gọi bằng lệnh `cmake`. **Không chép đường dẫn `~/tools/cmake-3.31.6-linux-x86_64` của Ubuntu cũ sang Debian** nếu Debian không có file đó. Dòng `export PATH=...` chỉ chỉnh nơi tìm chương trình, không cài CMake.

<a id="buoc-33"></a>

### 3.3. Tải NS-3 — hoặc dùng bản đã có

**Trong PuTTY, kiểm tra trước:**

```bash
ls -ld ~/ns-3-dev
```

**Nếu thư mục chưa tồn tại**, tải đúng phiên bản của bài:

```bash
cd ~
git clone --depth 1 --branch ns-3.48 https://gitlab.com/nsnam/ns-3-dev.git ns-3-dev
```

Chờ clone hoàn tất. Thông báo `detached HEAD` khi chọn phiên bản cố định không phải lỗi.

**Nếu thư mục đã tồn tại**, không clone lặp lại, không xóa thư mục. Kiểm tra:

```bash
cd ~/ns-3-dev
ls ns3 CMakeLists.txt VERSION
cat VERSION
```

Cần tìm thấy ba file và phiên bản `3.48` cho quy trình này. Nếu thiếu file hoặc phiên bản khác, kiểm tra bộ mã đang có trước khi tiếp tục.

<a id="buoc-34"></a>

### 3.4. Cấu hình và build NS-3

**Trong PuTTY:**

```bash
cd ~/ns-3-dev
```

Chép toàn bộ dòng cấu hình dưới đây rồi Enter:

```bash
./ns3 configure --build-profile=optimized --disable-examples --disable-tests --disable-python --disable-werror --enable-modules=lte,flow-monitor,netanim,applications,point-to-point,internet,mobility,csma,config-store
```

`configure` chọn các thành phần cần cho bài và chuẩn bị hệ thống build. `--disable-python` tắt Python bindings; vẫn cần Python 3 để chạy công cụ `./ns3`.

**Dấu hiệu xong:** `Configuring done`, `Generating done`, không có lỗi kết thúc.

Sau đó biên dịch:

```bash
./ns3 build -j 2
```

`-j 2` cho phép tối đa hai tác vụ biên dịch cùng lúc, không phải hai UE. Lần đầu có thể mất nhiều phút. Nếu có lỗi `Killed` do thiếu tài nguyên, kiểm tra RAM máy ảo và thử giảm xuống:

```bash
./ns3 build -j 1
```

**Dấu hiệu xong:** build kết thúc không lỗi và trở lại dấu nhắc. Những thư viện không đổi có thể được dùng lại; không cần lần nào cũng thấy chạy từ 0% đến 100%.

Không dùng `sudo ./ns3 ...`. Mục `netanim` trong danh sách module của NS-3 là phần hỗ trợ ghi XML; **chưa phải ứng dụng NetAnim có cửa sổ**.

<a id="buoc-35"></a>

### 3.5. Đưa mã bài vào NS-3, chạy và kiểm tra XML

**Trong PuTTY:** tải repository bài thực hành nếu chưa có `~/bai10-iot-ns3`:

```bash
cd ~
git clone https://github.com/Nuttnee/bai10-iot-ns3.git
```

Nếu thư mục đã có, dùng bộ mã hiện tại và kiểm tra file cần chạy, không clone lặp lại hoặc ghi đè các chỉnh sửa đang học.

Chép bản cơ bản vào `scratch` nếu chưa có; nếu đã sửa file đích, lưu bản riêng trước khi chép:

```bash
cp ~/bai10-iot-ns3/mophong_bai10.cc ~/ns-3-dev/scratch/
```

Kiểm tra:

```bash
ls -l ~/ns-3-dev/scratch/mophong_bai10.cc
```

Build mã bài:

```bash
cd ~/ns-3-dev
./ns3 build -j 2
```

Build thành công thì chạy:

```bash
./ns3 run scratch/mophong_bai10.cc
```

Bản này có thể chạy xong mà không in lời thông báo. Kiểm tra file và thời điểm cập nhật:

```bash
ls -lh mophong_bai10.xml mophong_bai10_flowmon.xml
```

| Tệp mới trong `~/ns-3-dev` | Cách dùng |
|---|---|
| `mophong_bai10.xml` | Mở bằng NetAnim để xem hình |
| `mophong_bai10_flowmon.xml` | Mở bằng trình soạn thảo hoặc `cat` để đọc thống kê |

```text
Sửa .cc trong scratch → lưu → build chương trình → run mô phỏng → XML → NetAnim
```

**Build không tự sinh XML. Run mới ghi XML nếu mã có phần xuất kết quả.** Không chạy `.cc` bằng cách chỉ nhập đường dẫn file; cách đó có thể báo `Permission denied`.

Tên XML nằm trong dòng `AnimationInterface anim("mophong_bai10.xml");` của mã. Đổi tên `.cc` không tự đổi tên XML.

**Dấu hiệu xong:** chạy không lỗi và hai XML có thời điểm cập nhật của lần chạy vừa rồi. Đây mới là kết quả trên Debian; XML tải từ GitHub là kết quả mẫu cũ.

<a id="buoc-36"></a>

### 3.6. Cài và build ứng dụng NetAnim

**Làm trong PuTTY.** Nếu đã build thành công và có `~/netanim-bai10/build/bin/netanim`, chuyển thẳng đến 3.7; không cần cài lại vì lỗi màn hình.

Cài thư viện Qt:

```bash
sudo apt update
sudo apt install -y build-essential cmake git qtbase5-dev
```

Tải NetAnim khi chưa có thư mục `~/netanim-bai10`:

```bash
cd ~
git clone --depth 1 --branch netanim-3.110 https://gitlab.com/nsnam/netanim.git netanim-bai10
```

Chuẩn bị cấu hình:

```bash
cmake -S ~/netanim-bai10 -B ~/netanim-bai10/build
```

`-S` là thư mục mã nguồn; `-B` là thư mục build. Cần thấy `Configuring done` và `Generating done` trước khi chạy:

```bash
cmake --build ~/netanim-bai10/build -j 2
```

**Dấu hiệu xong:** `[100%] Built target netanim` và lệnh kiểm tra sau tìm thấy file:

```bash
ls -lh ~/netanim-bai10/build/bin/netanim
```

Đã có chương trình không đồng nghĩa có phiên đồ họa để mở nó. Phần đó ở bước tiếp theo.

<a id="buoc-37"></a>

### 3.7. Debian mới chỉ có terminal: cài giao diện bằng lệnh

**Bản Debian tối giản dùng trong bài này ban đầu chỉ có màn hình terminal (`login:` / `tty1`), chưa có desktop.** Debian cũng có thể được cài kèm desktop ngay từ đầu; hướng dẫn dưới đây dành cho trường hợp chưa có giao diện.

NS-3 build và chạy được bằng terminal. **NetAnim là ứng dụng có cửa sổ**, nên muốn mở trực tiếp trong máy ảo thì cần cài giao diện Xfce và màn hình đăng nhập LightDM. Cài Qt / build NetAnim không tự cài đầy đủ desktop.

**Nơi nhập lệnh bước A–E:** terminal Debian trong VirtualBox, hoặc PuTTY đã SSH vào Debian. Không nhập các lệnh `sudo apt ...` trong PowerShell Windows. Chạy lần lượt từng khối; chỉ sang bước sau khi bước trước hoàn tất không lỗi. Máy đã có desktop thì chuyển sang mục 3.8.

#### A. Đăng nhập Debian và cập nhật danh sách gói

Nếu đang thấy `debian-iot login:`, nhập tài khoản Debian (máy minh họa là `nuthere`), Enter, rồi nhập mật khẩu. Mật khẩu không hiện ký tự khi gõ là bình thường. Khi thấy dấu nhắc dạng `nuthere@debian-iot:~$`, nhập:

```bash
sudo apt update
```

`sudo` chạy với quyền quản trị; nếu được hỏi mật khẩu, nhập mật khẩu tài khoản Debian. `apt update` chỉ cập nhật danh sách phần mềm, chưa cài giao diện.

#### B. Cài giao diện Xfce, LightDM và terminal đồ họa

```bash
sudo apt install task-xfce-desktop lightdm xfce4-terminal
```

- `task-xfce-desktop`: cài môi trường desktop Xfce và các thành phần liên quan.
- `lightdm`: tạo màn hình đăng nhập đồ họa.
- `xfce4-terminal`: cửa sổ Terminal để nhập lệnh sau khi vào desktop.

APT sẽ báo dung lượng cần tải và đĩa cần dùng. Nhập `Y` rồi Enter để cài. Nếu xuất hiện lựa chọn **Default display manager**, dùng phím mũi tên chọn **lightdm**, nhấn Tab tới **OK** rồi Enter. Đợi tới khi dấu nhắc lệnh xuất hiện lại; không khởi động lại khi APT còn chạy hoặc báo lỗi.

Gói desktop được mô tả trong [tài liệu gói chính thức của Debian 12](https://packages.debian.org/bookworm/task-xfce-desktop).

#### C. Kiểm tra đã cài đủ

```bash
dpkg -l task-xfce-desktop lightdm xfce4-terminal
```

Cả ba gói cần có ký hiệu **`ii`** đầu dòng. Nếu báo không tìm thấy gói hoặc trạng thái khác, xử lý lỗi cài đặt trước khi tiếp tục.

#### D. Cho máy khởi động vào giao diện

Chạy lần lượt:

```bash
sudo systemctl enable lightdm
```

Lệnh này bật dịch vụ màn hình đăng nhập khi Debian khởi động.

```bash
sudo systemctl set-default graphical.target
```

Lệnh này chọn chế độ khởi động đồ họa. **Chỉ chạy `set-default graphical.target` sẽ không tự cài desktop**; vẫn phải hoàn thành bước B.

#### E. Khởi động lại Debian

Lưu công việc và chờ các tiến trình build/cài đặt khác kết thúc, rồi nhập:

```bash
sudo reboot
```

**PuTTY ngắt kết nối sau lệnh này là bình thường.** Mở cửa sổ máy ảo Debian trong VirtualBox, chờ màn hình đăng nhập đồ họa, đăng nhập tài khoản Debian. Nếu được chọn phiên làm việc, chọn **Xfce Session**. Desktop có thanh menu và cửa sổ là dấu hiệu đã có giao diện.

PuTTY vẫn chỉ là cửa sổ SSH văn bản; giao diện desktop xuất hiện trong **VirtualBox**, không tự xuất hiện trong PuTTY.

#### F. Mở NetAnim từ desktop vừa cài

Trong cửa sổ Debian của VirtualBox, chọn **Applications → Terminal Emulator**, nhập:

```bash
~/netanim-bai10/build/bin/netanim
```

Lệnh này áp dụng sau khi đã build NetAnim ở mục 3.6. Khi cửa sổ NetAnim hiện ra, tiếp tục mục 3.8 để mở XML. Không chạy lệnh này bằng `sudo`.

#### Nếu khởi động lại vẫn chỉ thấy terminal

Đăng nhập tại terminal Debian hoặc kết nối lại bằng PuTTY, kiểm tra:

```bash
systemctl get-default
systemctl status lightdm --no-pager -l
```

Nếu LightDM đã cài nhưng đang `inactive`, khởi động dịch vụ:

```bash
sudo systemctl start lightdm
```

Sau đó xem lại cửa sổ VirtualBox. Nếu dịch vụ báo `failed`, lấy thông báo cụ thể:

```bash
sudo journalctl -u lightdm -b --no-pager -n 50
```

Lỗi `qt.qpa.xcb: could not connect to display` khi mở NetAnim trong PuTTY/tty có thể do chưa có phiên đồ họa. Không tự đặt `DISPLAY=:0` để thay cho việc cài và đăng nhập desktop; mở NetAnim từ Terminal bên trong desktop như bước F.

<a id="buoc-38"></a>

### 3.8. Mở NetAnim và nạp kết quả

**Làm trong desktop Debian ở cửa sổ VirtualBox, không phải PuTTY thông thường.**

Mở Applications → Terminal Emulator, nhập:

```bash
~/netanim-bai10/build/bin/netanim
```

Tên đúng là **netanim**, không phải `netaim` hoặc `metanim`.

Trong cửa sổ NetAnim:

1. Chọn tab **Animator**.
2. Bấm biểu tượng **thư mục** phía trên bên trái.
3. Đi tới thư mục cá nhân → `ns-3-dev`.
4. Chọn **`mophong_bai10.xml`**, bấm Open.
5. Khi thấy **`Parsing complete: Click Play`**, bấm tam giác xanh Play.

Muốn lấy đường dẫn đầy đủ để dán vào ô File name:

```bash
realpath ~/ns-3-dev/mophong_bai10.xml
```

Trên máy minh họa, kết quả là `/home/nuthere/ns-3-dev/mophong_bai10.xml`. Người dùng khác có tên tài khoản khác; không dùng đường dẫn `/home/user` của Ubuntu cũ.

**Dấu hiệu xong:** NetAnim đọc XML, hiện các node và phát được các sự kiện ghi lại. Không chọn `mophong_bai10_flowmon.xml` trong hộp mở hoạt họa.

<a id="buoc-39"></a>

### 3.9. Đọc thống kê và lưu kết quả Debian

Có thể quay lại PuTTY để đọc:

```bash
cd ~/ns-3-dev
cat mophong_bai10_flowmon.xml
```

Sao chép bằng chứng trước khi chạy bài khác:

```bash
mkdir -p ~/ket-qua-bai10-debian/lan1
cp ~/ns-3-dev/mophong_bai10.xml ~/ns-3-dev/mophong_bai10_flowmon.xml ~/ket-qua-bai10-debian/lan1/
```

Đổi `lan1` thành một tên mới cho mỗi thí nghiệm, tránh ghi đè kết quả trước. Ghi lại phiên bản bằng `cat /etc/debian_version`, `cmake --version`, `g++ --version` và `cat ~/ns-3-dev/VERSION`.

<a id="buoc-310"></a>

### 3.10. Các lỗi đã gặp và cách phân biệt

| Hiện tượng | Nguyên nhân / cách xử lý |
|---|---|
| PuTTY báo `Connection refused` | Kiểm tra máy ảo đang chạy, SSH active, PuTTY dùng cổng 2222 và quy tắc NAT chuyển tiếp sang cổng 22 |
| Dán lệnh xuất hiện `^[[200~` | Có ký tự điều khiển lẫn vào lệnh; Ctrl+C hủy dòng, nhập lại lệnh sạch |
| `destination path ... already exists` | Thư mục đã có; kiểm tra mã bên trong, không clone lặp hoặc xóa tùy tiện |
| Gõ đường dẫn `.cc` báo `Permission denied` | Dùng `./ns3 run scratch/mophong_bai10.cc` |
| `cmake: command not found` | Cài CMake trong Debian, không dùng đường dẫn bộ CMake của Ubuntu cũ |
| `netaim` / `metanim: command not found` | Gõ đúng đường dẫn `~/netanim-bai10/build/bin/netanim` |
| `could not connect to display` | Chưa có phiên đồ họa tại nơi gọi; làm mục 3.7 và mở từ desktop ở mục 3.8 |
| Đã là `graphical.target` nhưng chưa có desktop | Kiểm tra gói Xfce/LightDM; chỉ đổi target không tự cài giao diện |
| Build xong chưa có XML | Cần run chương trình |
| Sửa `.cc` mà hình vẫn cũ | Lưu đúng bản trong scratch, run lại và nạp lại XML |
| Chạy file `_setting.cc` nhưng XML không có `_setting` | Kiểm tra tên trong `AnimationInterface` và `SerializeToXmlFile` của mã |

**Mỗi buổi sau khi cài xong:** bật máy ảo → kết nối PuTTY để sửa/build/run → dùng desktop Debian để mở NetAnim. Không phải cài lại các phần mềm mỗi buổi.

---

<a id="ket-qua"></a>

## 4. Kết quả đo được

**Kết quả mẫu từ lần chạy Ubuntu/WSL2 trước khi chuyển môi trường.** Chưa coi các bảng dưới đây là kết quả đã đo lại trên Debian. Dùng XML mới sau mục 3.9 để đối chiếu và cập nhật.

### 4.1. Cấu hình cơ sở — 1 UE

Tệp bằng chứng: [`mophong_bai10_flowmon.xml`](mophong_bai10_flowmon.xml)

| flowId | Nguồn → Đích | Giao thức | Cổng đích | txPackets | rxPackets | lostPackets |
|---|---|---|---|---|---|---|
| 1 | 13.0.0.5 → 13.0.0.6 | UDP (17) | **2123** | 1 | 1 | 0 |
| 2 | 14.0.0.6 → 14.0.0.5 | UDP (17) | **2123** | 1 | 1 | 0 |
| 3 | 14.0.0.5 → 14.0.0.6 | UDP (17) | **2123** | 1 | 1 | 0 |
| 4 | 13.0.0.6 → 13.0.0.5 | UDP (17) | **2123** | 1 | 1 | 0 |

**Tổng: 4 luồng · 4 gói · 620 byte · 0 gói mất · trễ trung bình 125,5 ns**

### 4.2. Giải thích các thông số đề bài yêu cầu

| Thông số | Nghĩa | Giá trị đo được |
|---|---|---|
| `flowId` | số hiệu FlowMonitor gán cho mỗi **bộ năm** (IP nguồn, IP đích, giao thức, cổng nguồn, cổng đích) khác nhau | 1–4 |
| `sourceAddress` / `destinationAddress` | IP hai đầu của luồng | `13.0.0.x`, `14.0.0.x` — đường **nội bộ mạng lõi**, không phải IP của UE |
| `txPackets` / `rxPackets` | số gói gửi / nhận | 1 / 1 mỗi luồng |
| `txBytes` / `rxBytes` | tổng byte gửi / nhận | 146 hoặc 164 |
| `lostPackets` | số gói mất | 0 |
| `delaySum` | **tổng** độ trễ mọi gói nhận được; trễ trung bình = `delaySum / rxPackets` | 118–133 ns |
| `minDelay` / `maxDelay` | gói nhanh nhất / chậm nhất | bằng nhau, vì mỗi luồng chỉ có 1 gói |

---

## 5. Phát hiện quan trọng nhất

**Mã cơ sở chưa tạo lưu lượng ứng dụng; các luồng IP trong kết quả mẫu là báo hiệu mạng lõi.**

Các điểm để đối chiếu mã và số liệu:

1. **Cổng 2123 là GTP-C** — giao thức *điều khiển* của mạng lõi EPC. Dữ liệu người dùng phải
   đi qua GTP-U, **cổng 2152**. Không có luồng nào ở cổng 2152.
2. **Địa chỉ `13.0.0.x` và `14.0.0.x`** là hai đường point-to-point nội bộ giữa các thành phần
   mạng lõi do `PointToPointEpcHelper` tạo ra. UE được cấp IP trong dải `7.0.0.x` — dải này
   không xuất hiện trong kết quả.
3. **`minDelay` bằng `maxDelay` ở mọi luồng** — vì mỗi luồng chỉ có đúng một gói. Khi chỉ có
   một mẫu thì nhỏ nhất, lớn nhất và trung bình đều là chính nó.

**Nguyên nhân:** mã nguồn của bài **không cài ứng dụng sinh lưu lượng người dùng** — không `UdpClient`, không
`UdpServer`, không `OnOffApplication`. Nó chỉ dựng mạng rồi cho UE nhập mạng. FlowMonitor
không thể đo thứ không tồn tại, nên bốn luồng ghi nhận được đều là **bản tin thủ tục nhập mạng**.

> Nếu bị hỏi *"chạy xong sao không có dữ liệu?"* → trả lời: vì bài chưa có ứng dụng nào sinh
> lưu lượng; bốn luồng đó là thủ tục thiết bị nhập mạng, không phải dữ liệu.

**Hệ quả:** con số 118–133 **nano**giây là độ trễ của **đường dây trong mạng lõi**, không phải
độ trễ vô tuyến. Không được dùng nó để nói về độ trễ của NB-IoT (tài liệu ghi tới vài **giây**).

---

<a id="thi-nghiem"></a>

## 6. Ba thay đổi theo yêu cầu

### 6.1. Tăng số UE

Sửa `ueNodes.Create(1)` thành `ueNodes.Create(3)` — xem [`setting/mophong_bai10_setting.cc`](setting/mophong_bai10_setting.cc).

**Lưu ý bản setting đang lưu trong kho:** ba dòng đặt tọa độ đều dùng `ueNodes.Get(0)`, nên chỉ UE thứ nhất được đặt lại vị trí; hai UE còn lại chưa được đặt riêng. Muốn ba UE ở 20, 40, 60 m, sửa lần lượt thành `Get(0)`, `Get(1)`, `Get(2)`, lưu và chạy lại. Bảng dưới phản ánh XML đã lưu, không phải kết quả mới sau khi sửa lỗi này trên Debian. Mã setting vẫn xuất thống kê tên `mophong_bai10_flowmon.xml`, nên phải lưu riêng mỗi lần chạy để không ghi đè bản cơ sở.

| Số UE | Số luồng | txPackets | txBytes | Trễ TB | Gói mất |
|---|---|---|---|---|---|
| 1 | 4 | 4 | 620 | 125,5 ns | 0 |
| 3 | 4 | 12 | 1 860 | 158,8 ns | 0 |

**Nhận xét:**

* **Trong hai cấu hình đã lưu**, số gói tăng từ 4 lên 12, tương ứng `4 × số UE`. Không suy rộng thành quy luật cho mọi cấu hình LTE hoặc trường hợp kết nối thất bại.
* **Số luồng vẫn là 4, không tăng.** Vì FlowMonitor phân luồng theo *bộ năm* (IP, cổng), mà mọi
  UE đều dùng chung hai đường nội bộ của mạng lõi với cùng cổng 2123.
* **Trễ trung bình tăng 125,5 → 158,8 ns.** Việc nhiều bản tin dùng chung đường truyền có thể làm tăng chờ đợi; cần trace hàng đợi nếu muốn xác nhận nguyên nhân. Đây là thống kê mạng lõi, không phải phép đo độ trễ vô tuyến.

### 6.2. Đổi vị trí UE

**Cách làm trên Debian:** dùng bản cơ sở một UE; đặt trạm gốc tại `(0, 0)`, lần lượt đổi tọa độ UE thành `(20, 0)`, `(500, 0)` và `(2000, 0)`. Giữ nguyên thời gian và các tham số khác. Với mỗi trường hợp: lưu mã → build/run → lưu hai XML vào thư mục riêng.

Ghi nhận từ báo cáo trước cho biết các thống kê luồng vẫn là 4 luồng, 4 gói gửi/nhận, 620 byte và trễ trung bình 125,5 ns. **Các XML riêng theo khoảng cách chưa được đưa vào bảng bằng chứng của báo cáo này**, nên cần chạy lại và lưu chúng trên Debian trước khi xác nhận kết quả đó.

Cách đọc: nếu báo hiệu mạng lõi vẫn giống nhau, chỉ kết luận rằng **các thống kê được quan sát không đổi trong những lần chạy đó**. Không kết luận khoảng cách không ảnh hưởng tới LTE. Các luồng IP đang đo nằm trên các đường có dây trong mạng lõi; chúng không trực tiếp đo chất lượng đường vô tuyến.

Kiểm tra tọa độ UE trong XML hoạt họa để xác nhận sửa đúng vị trí. Nếu cần khẳng định UE hoàn tất kết nối, đối chiếu thêm trace trạng thái kết nối/RRC; riêng bốn luồng IP không phản ánh toàn bộ thủ tục vô tuyến.

### 6.3. Đổi thời gian mô phỏng

**Cách làm trên Debian:** giữ một UE cách trạm 20 m. Chạy ba lần và lưu kết quả riêng sau mỗi lần:

```bash
./ns3 run "scratch/mophong_bai10 --simTime=1050ms"
```

```bash
./ns3 run "scratch/mophong_bai10 --simTime=2s"
```

```bash
./ns3 run "scratch/mophong_bai10 --simTime=5s"
```

Các lệnh chạy tại `~/ns-3-dev`. Chúng ghi cùng tên XML, nên dùng mục 3.9 để sao chép kết quả trước khi chạy lần tiếp theo.

Báo cáo trước ghi nhận thống kê không đổi và gói cuối của các luồng IP được theo dõi ở khoảng 20,2148 ms. Cần XML của từng lần chạy Debian để xác nhận lại.

Mã chưa có ứng dụng sinh lưu lượng, nên tăng thời gian có thể không tăng số gói FlowMonitor ghi nhận. Điều đó không có nghĩa mô hình LTE đã hết mọi sự kiện nội bộ.

> `simTime` là **thời gian trong mô phỏng**, không phải thời gian chờ ngoài đời. Thời gian xử lý còn phụ thuộc số sự kiện và tài nguyên máy ảo.

### 6.4. Kết luận chung

Hai XML mẫu xác nhận số gói tăng khi đổi từ một lên ba UE. Hai thí nghiệm về khoảng cách và thời gian cần bổ sung bằng chứng riêng trên Debian. Dù thống kê báo hiệu có thể không đổi, vẫn cần ghi nhận kết quả thực tế thay vì suy đoán.

Muốn đánh giá thông lượng, tỉ lệ nhận gói và độ trễ đầu cuối của lưu lượng người dùng, cần thêm ứng dụng, chẳng hạn `UdpClient` trên UE và `UdpServer` trên máy chủ ngoài EPC, đồng thời cấu hình địa chỉ và định tuyến. Độ trễ đầu cuối này bao gồm nhiều đoạn mạng; muốn tách riêng độ trễ vô tuyến cần phép đo phù hợp.

---

## 7. Giới hạn của bài

**Đây là mô phỏng LTE, không phải NB-IoT.** NS-3 dùng mô hình LTE/EPC. Không có kênh 180 kHz,
không có lặp truyền, không có PSM/eDRX, không tính tiêu thụ điện. **Không rút được kết luận nào
về vùng phủ hay tuổi thọ pin của NB-IoT** từ bài này.

**Không dùng báo hiệu làm bằng chứng dữ liệu ứng dụng đã tới.** Bốn luồng GTP-C cho thấy trao đổi báo hiệu mạng lõi; để xác nhận trạng thái kết nối UE cần đối chiếu trace phù hợp. Muốn chứng minh dữ liệu ứng dụng tới máy chủ phải kiểm tra tại ứng dụng nhận.

**NetAnim chủ yếu giúp xem node, liên kết và sự kiện gói tin được ghi.** Khả năng hiện thông tin gói phụ thuộc trace và cấu hình. FlowMonitor cung cấp thống kê luồng, không phải nội dung payload như số đo cảm biến. Bài này chưa có payload ứng dụng để đọc.

---

## 8. Tệp bằng chứng trong kho

| Tệp | Nội dung |
|---|---|
| [`mophong_bai10.cc`](mophong_bai10.cc) | mã nguồn bài cơ sở, 1 UE |
| [`mophong_bai10.xml`](mophong_bai10.xml) | dữ liệu NetAnim |
| [`mophong_bai10_flowmon.xml`](mophong_bai10_flowmon.xml) | thống kê FlowMonitor, 1 UE |
| [`setting/mophong_bai10_setting.cc`](setting/mophong_bai10_setting.cc) | bản sửa thành 3 UE |
| [`setting/mophong_bai10_flowmon.xml`](setting/mophong_bai10_flowmon.xml) | thống kê FlowMonitor, 3 UE |

Các bảng 1 UE và 3 UE có XML thống kê tương ứng trong bảng trên. Phần so sánh khoảng cách/thời gian cần bổ sung các tệp bằng chứng riêng. Khi chuyển sang Debian, lưu XML và cấu hình mới trước khi cập nhật kết luận.

---

## 9. Tài liệu tham khảo

* [NS-3 — hướng dẫn cài đặt](https://www.nsnam.org/docs/installation/singlehtml/)
* [NS-3 — mã nguồn phiên bản 3.48](https://gitlab.com/nsnam/ns-3-dev/-/tree/ns-3.48)
* [NetAnim — mã nguồn phiên bản 3.110](https://gitlab.com/nsnam/netanim/-/tree/netanim-3.110)
* [Debian 12 — gói desktop Xfce](https://packages.debian.org/bookworm/task-xfce-desktop)
* [VirtualBox — chuyển tiếp cổng NAT](https://www.virtualbox.org/manual/ch06.html#natforward)

* R. Herrero, *Practical Internet of Things Networking*, Springer, 2023
* R. Herrero, *Fundamentals of IoT Communication Technologies*, Springer
* D. Hanes và cộng sự, *IoT Fundamentals: Networking Technologies, Protocols and Use Cases for IoT*, Cisco Press, 2017
* GSMA, *Mobile IoT in a 5G Future*, 10/2024
* [Tài liệu NS-3](https://www.nsnam.org/documentation/) · [3GPP Release 13](https://www.3gpp.org/release-13)
---

## 10. Xác nhận chạy trên Debian ngày 30/09/2026

Đã kết nối bằng SSH đến Debian, build thành công năm file C++ và chạy bảy kịch bản. Cả 14 file XML sinh ra đều được đọc lại bằng trình phân tích XML. Đây là kết quả kiểm tra mới trên Debian, tách biệt với bộ XML Ubuntu mẫu ở mục 4–6.

| Kịch bản | Số UE / thời gian | Gói IP gửi / nhận trong FlowMonitor |
|---|---|---|
| LTE cơ bản | 1 UE / 1,05 giây | 4 / 4 |
| LTE có chú thích | 1 UE / 1,05 giây | 4 / 4 |
| Tăng số UE | 3 UE / 1,05 giây | 12 / 12 |
| Đổi khoảng cách | 1 UE cách 100 m / 1,05 giây | 4 / 4 |
| Đổi thời gian | 1 UE cách 20 m / 5 giây | 4 / 4 |
| Đồng hồ nước | 3 UE / 10 giây | 36 / 36, gồm cả báo hiệu |
| Setting gốc | 3 UE / 1,05 giây | 12 / 12 |

Riêng demo đồng hồ nước: CSV ở tầng ứng dụng ghi **mỗi đồng hồ gửi 8, nhận đủ 8 bản tin**, không có bản tin trùng; tổng 24 bản tin ứng dụng. Không lấy tổng 36 gói của FlowMonitor làm số bản tin đo nước vì còn có báo hiệu. Độ trễ ứng dụng trung bình của lần chạy này là 22,0538 ms cho mỗi đồng hồ; đây là số của kịch bản cụ thể, không phải cam kết độ trễ mạng LTE thực tế.

Bộ bài đã chép trực tiếp vào máy Debian đang thực hành:

- Mã được NS-3 build: `~/ns-3-dev/scratch/`.
- Hướng dẫn và lệnh chọn bài: `~/bai10-thuc-hanh/`.
- XML, CSV và log từng lần chạy: `~/bai10-thuc-hanh/ket-qua/`.
- Bảng kiểm tra XML: `~/bai10-thuc-hanh/kiem-tra-ket-qua.json`.

Để xem danh sách trên máy đã chuẩn bị:

```bash
bash ~/bai10-thuc-hanh/chay-bai.sh
```

Để chạy demo đồng hồ nước:

```bash
bash ~/bai10-thuc-hanh/chay-bai.sh 6
```

Các đường dẫn trên mô tả **máy Debian đã được chuẩn bị trực tiếp**, không tự xuất hiện khi người khác chỉ clone repository này. Người cài mới thực hiện mục 3 để chạy bài LTE cơ bản trong repository.

Trong lần kiểm tra đã phát hiện các object `.o` cũ có dung lượng 0 byte làm linker báo `undefined reference`. Đã cho biên dịch lại các object rỗng và build thành công. Kiểm tra này xác nhận chương trình chạy và tạo XML; chưa thay thế bước người học mở cửa sổ NetAnim trong desktop Debian để quan sát hoạt họa.
