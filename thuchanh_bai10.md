# Thực hành Bài 10 trên Debian: từ code C++ đến XML và NetAnim

Hướng dẫn dành cho người mới, dùng tên **`bai10-cua-toi.cc`** xuyên suốt. Thực hiện lần lượt từng bước, không dán toàn bộ các lệnh một lúc.

**Điều kiện:** đã cài NS-3 tại `~/ns-3-dev` và NetAnim tại `~/netanim-bai10/build/bin/netanim`. Tài khoản minh họa là `nuthere`; nếu bạn dùng tài khoản khác, thay `/home/nuthere` bằng thư mục cá nhân của mình. Ký hiệu `~` tự trỏ tới thư mục cá nhân của tài khoản đang đăng nhập.

Nếu Debian chỉ có terminal, chưa có desktop, làm [phần cài giao diện bằng lệnh](BAO_CAO.md#buoc-37) trước. Nếu chưa có phần mềm, xem [hướng dẫn cài NS-3 và NetAnim](BAO_CAO.md#buoc-32). Nếu muốn dùng PuTTY, xem [hướng dẫn SSH](BAO_CAO.md#buoc-31).

> **Quy trình:** mở Debian → mở Terminal → tạo/sửa `.cc` → lưu → build → run → XML → NetAnim.
>
> **Build tạo chương trình chạy được. Run mới chạy mô phỏng và ghi XML.**

## Tra cứu nhanh

| Bạn đang cần | Đến bước |
|---|---|
| Mở Debian và Terminal | [1–2](#mo-debian) |
| Tạo file, copy code và lưu | [3–7](#tao-code) |
| Build và chạy để tạo XML | [8–9](#build-run) |
| Mở XML trong NetAnim | [10](#mo-xml) |
| Tăng số UE | [11](#tang-ue) |
| Đổi vị trí UE | [12](#doi-vi-tri) |
| Đổi thời gian mô phỏng | [13](#doi-thoi-gian) |
| Đọc thống kê và so sánh | [14](#thong-ke) |
| Gặp lỗi | [Bảng xử lý lỗi](#loi) |

<a id="mo-debian"></a>

## 1. Mở máy ảo Debian

Trên Windows, mở **VirtualBox** → chọn máy Debian → bấm **Start**. Đăng nhập tài khoản Debian, ví dụ `nuthere`, rồi chờ desktop hiện ra.

Các thao tác bên dưới thực hiện **trong Debian**, không nhập lệnh Linux vào PowerShell Windows.

## 2. Mở Terminal

Trong desktop Debian, chọn **Applications → Terminal Emulator**. Bạn sẽ thấy dấu nhắc tương tự:

```text
nuthere@debian-iot:~$
```

Chỉ nhập các lệnh trong ô bên dưới; **không gõ lại phần `nuthere@debian-iot:~$`**. Nhấn Enter sau mỗi lệnh.

<a id="tao-code"></a>

## 3. Kiểm tra thư mục NS-3

```bash
cd ~/ns-3-dev
```

Lệnh `cd` chuyển vào thư mục NS-3. Kiểm tra vị trí:

```bash
pwd
```

Máy minh họa sẽ in `/home/nuthere/ns-3-dev`. Kiểm tra thư mục chứa mã bài thực hành:

```bash
ls -ld scratch
```

Nếu báo `No such file or directory`, kiểm tra lại nơi bạn cài NS-3 trước khi tiếp tục.

## 4. Tạo file `bai10-cua-toi.cc`

Nhập trong Terminal:

```bash
mousepad ~/ns-3-dev/scratch/bai10-cua-toi.cc
```

| Phần lệnh | Ý nghĩa |
|---|---|
| `mousepad` | Mở trình soạn thảo có giao diện |
| `~` | Thư mục cá nhân, ở đây là `/home/nuthere` |
| `ns-3-dev/scratch/` | Nơi chứa mã bài thực hành |
| `bai10-cua-toi.cc` | File mã C++ bạn muốn mở hoặc tạo |

**Nếu file chưa tồn tại:** Mousepad mở trang soạn thảo trống; nếu hỏi tạo file, chọn **Create/Tạo**. Bạn sẽ dán code rồi lưu ở bước 5–7. Mở trang trống chưa có nghĩa là code đã được lưu.

**Nếu file đã tồn tại:** Mousepad hiển thị code cũ. Muốn sửa bài đó thì sửa rồi lưu. Muốn giữ nguyên bài cũ, chọn **File → Save As**, dùng tên mới và vẫn lưu trong `scratch`. Nếu đổi tên, các lệnh build/run sau phải dùng đúng tên của bạn.

Nếu báo `mousepad: command not found`, cài bằng hai lệnh, chờ từng lệnh hoàn tất:

```bash
sudo apt update
sudo apt install mousepad
```

Nhập `Y` nếu được hỏi, rồi chạy lại lệnh mở Mousepad. Nếu Terminal cũ đang chờ Mousepad, giữ cửa sổ soạn thảo và mở thêm Terminal; hoặc lưu rồi đóng Mousepad để dùng tiếp Terminal cũ.

## 5. Đưa code mẫu vào file

Dùng [mã LTE cơ bản `mophong_bai10.cc`](mophong_bai10.cc) trong repository này để các bước sửa bên dưới khớp với code.

**Cách A — máy bạn đã có mã mẫu trong `scratch`:**

1. Trong Mousepad, nhấn **Ctrl + O**.
2. Mở `/home/nuthere/ns-3-dev/scratch/mophong_bai10.cc`.
3. Nhấn **Ctrl + A**, rồi **Ctrl + C** để copy toàn bộ mã.
4. Chuyển về tab/cửa sổ **`bai10-cua-toi.cc`**.
5. Bấm vào trang trắng và nhấn **Ctrl + V**.

**Cách B — máy chưa có mã mẫu:** mở liên kết mã mẫu phía trên trong trình duyệt, chọn **Raw**, copy toàn bộ nội dung rồi dán vào `bai10-cua-toi.cc` trong Mousepad. Nếu chuyển nội dung từ Windows sang Debian bằng clipboard không được, mở trang GitHub trực tiếp trong trình duyệt Debian hoặc chuyển file `.cc` sang máy ảo trước.

Chỉ dán mã C++, không dán phần giải thích, số thứ tự dòng hoặc dấu bao Markdown như ` ```cpp ` và ` ``` `. Không dán thêm cả chương trình vào cuối file đã có code, vì có thể làm trùng hàm `main`.

Nếu đã có nguyên file `.cc`, có thể chép file vào `scratch`, không cần tạo trang trống rồi dán lại.

## 6. Đặt tên XML đầu ra

Kiểm tra tên tab đang sửa là **`bai10-cua-toi.cc`**. Nhấn **Ctrl + F**, tìm `AnimationInterface anim`.

Đổi dòng:

```cpp
AnimationInterface anim("mophong_bai10.xml");
```

thành:

```cpp
AnimationInterface anim("bai10-cua-toi.xml");
```

Tiếp tục tìm `SerializeToXmlFile`, thay dòng xuất thống kê bằng:

```cpp
monitor->SerializeToXmlFile("bai10-cua-toi-flowmon.xml", true, true);
```

| File | Vai trò |
|---|---|
| `bai10-cua-toi.cc` | Mã C++ bạn sửa |
| `bai10-cua-toi.xml` | Hoạt họa mạng để mở ở tab Animator của NetAnim |
| `bai10-cua-toi-flowmon.xml` | Thống kê gói tin để đọc bằng trình soạn thảo |

**Đổi tên `.cc` không tự đổi tên XML.** Tên XML do hai dòng code trên quyết định. Lúc này chưa chạy mô phỏng nên chưa có hai XML mới.

## 7. Lưu và kiểm tra file code

Nhấn **Ctrl + S** trong Mousepad. Nếu hiện hộp thoại lưu:

1. Ô **Name/Tên**: nhập `bai10-cua-toi.cc`.
2. Chọn thư mục `/home/nuthere/ns-3-dev/scratch/`.
3. Bấm **Save/Lưu**. Không lưu thành `.cc.txt`.

Sau đó đóng Mousepad hoặc mở thêm Terminal. Kiểm tra:

```bash
ls -lh ~/ns-3-dev/scratch/bai10-cua-toi.cc
```

Cần thấy tên file và dung lượng khác 0. Bạn cũng có thể mở trình quản lý file, đi theo **Home → ns-3-dev → scratch**:

```text
/home/nuthere/
└── ns-3-dev/
    └── scratch/
        └── bai10-cua-toi.cc
```

<a id="build-run"></a>

## 8. Build chương trình

Quay lại Terminal:

```bash
cd ~/ns-3-dev
```

Rồi chạy:

```bash
./ns3 build -j 2
```

| Phần lệnh | Ý nghĩa |
|---|---|
| `./ns3` | Công cụ quản lý NS-3 trong thư mục hiện tại |
| `build` | Biên dịch mã C++ thành chương trình chạy được |
| `-j 2` | Cho phép tối đa hai công việc biên dịch song song |

Lệnh build xử lý các mục đã cấu hình, bao gồm bài mới trong `scratch`. Những phần không đổi thường không cần biên dịch lại.

Chờ lệnh hoàn tất. Nếu có lỗi như `error:`, `FAILED` hoặc `build stopped`, xử lý lỗi trước khi chạy tiếp. **Build thành công chưa tạo XML.**

## 9. Chạy đúng bài để tạo XML

Vẫn ở thư mục `~/ns-3-dev`, nhập:

```bash
./ns3 run scratch/bai10-cua-toi.cc
```

Lệnh này chọn đúng bài `bai10-cua-toi.cc` để chạy. Mã mẫu có thể chạy xong mà không in thông báo. Kiểm tra:

```bash
ls -lh bai10-cua-toi.xml bai10-cua-toi-flowmon.xml
```

Hai file cần tồn tại và có thời điểm cập nhật của lần chạy vừa rồi. Với tên đã sửa ở bước 6 và cách chạy này, chúng nằm tại:

```text
/home/nuthere/ns-3-dev/bai10-cua-toi.xml
/home/nuthere/ns-3-dev/bai10-cua-toi-flowmon.xml
```

> Hướng dẫn này chạy trực tiếp từ `~/ns-3-dev`, nên XML nằm ở đó. Script chọn bài `~/bai10-thuc-hanh/chay-bai.sh` đã chuẩn bị riêng trên máy minh họa dùng thư mục `~/bai10-thuc-hanh/ket-qua/`. Đó là hai cách chạy khác nhau; script này không tự có trên máy mới chỉ clone repository.

<a id="mo-xml"></a>

## 10. Mở XML bằng NetAnim

Nhập **trong Terminal của desktop Debian**, không phải PuTTY thông thường:

```bash
~/netanim-bai10/build/bin/netanim
```

Trong cửa sổ NetAnim:

1. Chọn tab **Animator**.
2. Bấm biểu tượng **thư mục** phía trên bên trái.
3. Tìm tới `/home/nuthere/ns-3-dev/`.
4. Chọn **`bai10-cua-toi.xml`** → **Open**.
5. Khi thấy mô hình và dòng **Parsing complete**, bấm **▶ Play**.

Nếu hộp chọn file hỗ trợ **Ctrl + L**, dùng tổ hợp đó rồi nhập đường dẫn đầy đủ `/home/nuthere/ns-3-dev/bai10-cua-toi.xml`; hoặc nhập vào ô tên file.

**Không chọn `bai10-cua-toi-flowmon.xml` trong hộp mở hoạt họa.** File đó chứa thống kê, không chứa mô hình hoạt họa. Bản NetAnim chưa có bản sửa kiểm tra file có thể bị văng khi chọn nhầm loại này.

Mã cơ bản mô phỏng một UE kết nối với trạm gốc và mạng lõi EPC. Chưa có ứng dụng gửi dữ liệu cảm biến liên tục; không cần ESP32 hay Arduino thật.

<a id="tang-ue"></a>

## 11. Thực hành tăng số UE

**Trước mỗi thay đổi cần so sánh, lưu kết quả cũ.** Ví dụ sau lần chạy cơ bản ở bước 9:

```bash
cd ~/ns-3-dev
lan=$(mktemp -d "$HOME/ket-qua-bai10-XXXXXX")
cp bai10-cua-toi.xml bai10-cua-toi-flowmon.xml "$lan/"
printf 'Da luu ket qua tai: %s\n' "$lan"
```

Mỗi lần chạy khối này tạo thư mục mới, tránh ghi đè bản so sánh. Ghi lại thông số tương ứng của lần chạy.

Mở code:

```bash
mousepad ~/ns-3-dev/scratch/bai10-cua-toi.cc
```

Tìm dòng `ueNodes.Create(1);`, đổi thành:

```cpp
ueNodes.Create(3);
```

Tìm dòng `anim.SetConstantPosition(ueNodes.Get(0), 20.0, 0.0);`. Thay **dòng đó** bằng ba dòng:

```cpp
anim.SetConstantPosition(ueNodes.Get(0), 20.0, 0.0);
anim.SetConstantPosition(ueNodes.Get(1), 40.0, 0.0);
anim.SetConstantPosition(ueNodes.Get(2), 60.0, 0.0);
```

| Đoạn code | Thiết bị | Tọa độ (m) |
|---|---|---|
| `ueNodes.Get(0)` | UE thứ nhất | (20, 0) |
| `ueNodes.Get(1)` | UE thứ hai | (40, 0) |
| `ueNodes.Get(2)` | UE thứ ba | (60, 0) |

C++ đếm từ 0. **Không viết cả ba dòng đều là `Get(0)`**, vì như vậy chỉ đổi vị trí cùng một UE. Chỉ dùng `Get(1)` và `Get(2)` sau khi đã tạo đủ ba UE.

Nhấn **Ctrl + S**, quay lại Terminal rồi chạy từng lệnh:

```bash
cd ~/ns-3-dev
./ns3 build -j 2
./ns3 run scratch/bai10-cua-toi.cc
```

Nạp lại `bai10-cua-toi.xml` trong NetAnim. Với mô hình này trên NS-3 3.48, dự kiến có **7 node: 3 UE, 1 trạm gốc và 3 thành phần EPC**. Số in cạnh node là mã định danh, không tự nói lên vai trò của nó.

<a id="doi-vi-tri"></a>

## 12. Thực hành thay đổi vị trí UE

Lưu kết quả bài 11 trước khi chạy tiếp bằng khối sao lưu ở đầu bước 11. Giữ ba UE, mở lại file code và đổi vị trí UE thứ ba từ:

```cpp
anim.SetConstantPosition(ueNodes.Get(2), 60.0, 0.0);
```

thành:

```cpp
anim.SetConstantPosition(ueNodes.Get(2), 100.0, 30.0);
```

UE thứ ba có tọa độ **x = 100 m, y = 30 m**. Đây là tọa độ, không phải khoảng cách đúng 100 m tới trạm ở (0, 0). Đây cũng là **vị trí cố định mới**, chưa phải chuyển động trong lúc mô phỏng.

Lưu **Ctrl + S**, build rồi run:

```bash
cd ~/ns-3-dev
./ns3 build -j 2
./ns3 run scratch/bai10-cua-toi.cc
```

Mở lại XML trong NetAnim, đối chiếu vị trí UE thứ ba. Không kết luận chất lượng đường vô tuyến chỉ từ việc thấy node nằm xa hơn trên hình; cần lưu lượng ứng dụng và phép đo phù hợp để đánh giá.

<a id="doi-thoi-gian"></a>

## 13. Thực hành thay đổi thời gian mô phỏng

Lưu kết quả bài 12 trước khi chạy tiếp. Mở code và tìm:

```cpp
Time simTime = MilliSeconds(1050);
```

Đổi thành:

```cpp
Time simTime = Seconds(5);
```

Thời gian mô phỏng đổi từ **1,05 giây thành 5 giây**. Đây là thời gian trong mô hình, không phải cam kết chương trình mất đúng 5 giây thực để chạy.

Lưu, build rồi run:

```bash
cd ~/ns-3-dev
./ns3 build -j 2
./ns3 run scratch/bai10-cua-toi.cc
```

Nạp lại XML trong NetAnim. **Mô phỏng dài hơn không bảo đảm có thêm mũi tên/gói tin.** Mã cơ bản chưa có ứng dụng truyền liên tục nên sự kiện có thể chỉ tập trung lúc thiết lập kết nối.

Các bước 11–13 đang sửa nối tiếp: sau cùng là 3 UE, UE thứ ba ở (100, 30), thời gian 5 giây. Khi so sánh tác động riêng của một thông số, giữ nguyên các thông số khác giữa hai lần chạy.

<a id="thong-ke"></a>

## 14. Đọc thống kê và ghi nhận xét

Mở thống kê bằng Mousepad:

```bash
mousepad ~/ns-3-dev/bai10-cua-toi-flowmon.xml
```

Nhấn **Ctrl + F** để tìm:

| Thông số | Ý nghĩa |
|---|---|
| `txPackets`, `rxPackets` | Số gói đã gửi, đã nhận của một luồng |
| `txBytes`, `rxBytes` | Tổng byte đã gửi, đã nhận của một luồng |
| `delaySum` | Tổng độ trễ các gói nhận được |
| `minDelay`, `maxDelay` | Độ trễ nhỏ nhất, lớn nhất nếu có trong đầu ra |
| `flowId` | Mã định danh luồng |
| `sourceAddress`, `destinationAddress` | IP nguồn, IP đích của luồng |

Độ trễ trung bình = `delaySum / rxPackets`, chỉ tính khi `rxPackets > 0`, chú ý đơn vị thời gian. Dùng cùng `flowId` để đối chiếu phần thống kê với phần địa chỉ trong XML.

Với mã LTE cơ bản này, FlowMonitor chủ yếu ghi báo hiệu mạng lõi. Không coi đó là phép đo dữ liệu cảm biến từ UE tới máy chủ. Bài dùng LTE/EPC, không phải mô hình đầy đủ NB-IoT hay phép đo tuổi thọ pin.

Ghi lại kết quả thực tế của bạn:

| Lần chạy | Số UE | Vị trí UE | Thời gian | File kết quả đã lưu | Quan sát / số liệu |
|---|---|---|---|---|---|
| Cơ bản | 1 | (20, 0) | 1,05 s | Điền đường dẫn | Điền kết quả |
| Tăng UE | 3 | (20, 0), (40, 0), (60, 0) | 1,05 s | Điền đường dẫn | Điền kết quả |
| Đổi vị trí | 3 | UE thứ ba (100, 30) | 1,05 s | Điền đường dẫn | Điền kết quả |
| Đổi thời gian | 3 | Giữ vị trí lần trước | 5 s | Điền đường dẫn | Điền kết quả |

<a id="loi"></a>

## Các lỗi thường gặp

| Hiện tượng | Cách xử lý |
|---|---|
| `mousepad: command not found` | Cài Mousepad theo bước 4 |
| Terminal chưa hiện dấu nhắc khi Mousepad/NetAnim đang mở | Mở thêm Terminal, hoặc lưu rồi đóng ứng dụng |
| `./ns3: No such file or directory` | Chạy `cd ~/ns-3-dev` trước |
| Gõ đường dẫn `.cc` báo `Permission denied` | Dùng `./ns3 run scratch/bai10-cua-toi.cc`; không chạy file nguồn trực tiếp |
| Không tìm thấy bài để run | Kiểm tra file nằm trong `scratch`, đã lưu và đuôi là `.cc` |
| `cmake: command not found` | Cài CMake trong Debian theo hướng dẫn cài đặt; không dùng đường dẫn CMake của Ubuntu cũ |
| Build thành công nhưng chưa có XML | Làm bước 9; XML được tạo khi run |
| XML có tên khác dự kiến | Kiểm tra hai tên đầu ra trong code ở bước 6 |
| NetAnim báo `could not connect to display` | Mở từ Terminal trong desktop Debian; xem hướng dẫn giao diện nếu chưa có desktop |
| Chọn XML rồi NetAnim văng | Kiểm tra đã chọn XML hoạt họa, không phải FlowMonitor; nếu đúng file vẫn lỗi, lưu tên file và thông báo để chẩn đoán |
| Hình không đổi sau khi sửa code | Ctrl+S, build/run lại, rồi nạp lại đúng XML vừa cập nhật |

**Mỗi lần sửa bài:** mở `.cc` → sửa → Ctrl+S → build → run → mở lại XML. Không cần cài lại NS-3 hoặc NetAnim.
