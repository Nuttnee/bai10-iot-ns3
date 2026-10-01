# Bài 10: mô hình LTE ba UE có chú thích

Gói này gồm XML hoạt họa đã chạy bằng ns-3.48 và tài liệu giải thích số liệu. Máy khác chỉ cần NetAnim để xem XML; không cần build lại NS-3 để xem kết quả có sẵn. Đây là LTE/EPC gia nhập mạng, chưa có ứng dụng gửi số đo và không phải mô hình đầy đủ NB-IoT/LTE-M.

## 1. Tải và mở trên máy Debian / Ubuntu khác

Mở Terminal và chạy lần lượt:

```bash
sudo apt update
sudo apt install -y curl
mkdir -p ~/BAI_XML_CHU_THICH
cd ~/BAI_XML_CHU_THICH
curl -fL --retry 2 -o mophong_3ue_chuthich_NETANIM.xml https://raw.githubusercontent.com/Nuttnee/bai10-iot-ns3/main/BAI_XML_CHU_THICH/mophong_3ue_chuthich_NETANIM.xml
curl -fL --retry 2 -o GIAI_THICH_VA_CACH_MO.md https://raw.githubusercontent.com/Nuttnee/bai10-iot-ns3/main/BAI_XML_CHU_THICH/GIAI_THICH_VA_CACH_MO.md
ls -lh
```

Kho mã công khai, tải hai file này không cần tài khoản/mật khẩu GitHub. Nếu đã có curl thì bỏ qua hai lệnh cài đặt đầu. Chạy lại các lệnh tải sẽ cập nhật hai file cùng tên.

Trong desktop Debian, mở NetAnim bằng cách đã cài trên máy. Nếu dùng đúng thư mục cài trong hướng dẫn bài 10:

```bash
~/netanim-bai10/build/bin/netanim
```

Trong NetAnim: chọn **Animator → biểu tượng thư mục → Home → BAI_XML_CHU_THICH → mophong_3ue_chuthich_NETANIM.xml → Open → Play**.

- Mở XML từ bên trong NetAnim. Nhấp đúp XML trong trình quản lý file có thể mở Firefox và hiện mã thô; đó không phải lỗi file.
- Nếu thiếu nhãn, bật **Show Node Id**. Kéo tốc độ về **slow**, phóng to và tắt lớp tọa độ/lưới nếu chữ bị chồng.
- Để đọc tài liệu trên máy, dùng `mousepad ~/BAI_XML_CHU_THICH/GIAI_THICH_VA_CACH_MO.md` nếu đã có Mousepad, hoặc đọc trực tiếp trên GitHub.
- Gói này không chứa mã C++, CSV hay FlowMonitor; các thông số và bảng dưới đây đã được chép từ lần chạy trên Debian gốc. Chỉnh sửa XML không chạy lại mô phỏng. Muốn đổi tham số và đo lại cần mã C++ cùng NS-3.
- Không mở FlowMonitor XML trong Animator. Ở bài cũ, `lte-3ue.xml` là hoạt họa, còn `lte-lab1-3ue.xml` là thống kê; hai tên đó dễ gây nhầm.

## 2. Bản này biểu diễn điều gì?

Ba thiết bị UE đăng ký với một trạm LTE. Mạng lõi trao đổi báo hiệu để thiết lập kết nối và thông tin phiên cho từng thiết bị.

Chưa có máy chủ ngoài EPC, chưa có ứng dụng UDP/TCP gửi số đo. Vì thế các gói FlowMonitor ghi ở đây là **báo hiệu GTP-C của mạng lõi**, không phải bản tin cảm biến truyền tới máy chủ.

Đây là mô hình **LTE/EPC thông thường** phục vụ bài thực hành. Nó không mô phỏng đầy đủ đặc tính riêng NB-IoT hay LTE-M: không dùng kết quả này để chứng minh 180 kHz, MCL 164 dB, pin 10 năm hoặc PSM/eDRX.

## 3. Từng nút trong hình

| ID trong lần chạy này | Nhãn / màu | Nhiệm vụ |
|---|---|---|
| 4 | UE1 — xanh lá | Thiết bị LTE thứ nhất, IP 7.0.0.2 |
| 5 | UE2 — xanh lá | Thiết bị LTE thứ hai, IP 7.0.0.3 |
| 6 | UE3 — xanh lá | Thiết bị LTE thứ ba, IP 7.0.0.4 |
| 3 | eNB — xanh dương | Trạm gốc thu/phát LTE, phục vụ cả ba UE |
| 1 | S-GW — cam | Gateway chuyển tiếp; phối hợp thiết lập đường truyền với MME và P-GW |
| 0 | P-GW — xanh ngọc | Gateway hướng tới mạng IP ngoài; bản này chưa nối máy chủ ngoài |
| 2 | MME — tím | Quản lý đăng ký/kết nối, điều phối thiết lập phiên |

Số trong ngoặc vuông là **node ID** do NS-3 cấp theo thứ tự tạo. Nó không phải số thiết bị đang dùng, mức tín hiệu, tốc độ hoặc số gói.

**Node ID 3**, **cell ID 1** và **flow ID 1** là ba loại mã khác nhau: mã nút, mã cell và mã luồng. Đừng đồng nhất chúng.

Trong helper NS-3 này, HSS/PCRF không được tạo thành các node riêng như sơ đồ mạng thương mại đầy đủ. Không thêm các nút giả để làm hình trông đầy đủ hơn.

## 4. Các đường nối, mũi tên và tọa độ

| Đường | Ý nghĩa |
|---|---|
| UE và eNB | Liên lạc vô tuyến LTE, không phải dây Point-to-Point; không có dây vẽ không đồng nghĩa mất kết nối |
| eNB — S-GW, S1-U | Đường dữ liệu từ trạm vào EPC; bản này chưa có dữ liệu ứng dụng chạy qua |
| S-GW — P-GW, S5 | Nối hai gateway; trong bản này có trao đổi báo hiệu thiết lập phiên |
| MME — S-GW, S11 | Báo hiệu quản lý phiên |

Mũi tên di chuyển biểu diễn các gói được ghi vào trace. Không phải mọi thủ tục LTE đều được vẽ thành mũi tên: helper dùng một số giao tiếp nội bộ, ví dụ S1-MME, thay vì một đường IP độc lập để FlowMonitor quan sát.

Các cặp số dạng `0,50` trên lưới là tọa độ X/Y. Đối với UE/eNB, vị trí có ý nghĩa vật lý trong mô hình. Với MME, S-GW, P-GW, vị trí chỉ để sắp xếp hình; đường truyền có dây được cấu hình bằng tốc độ/độ trễ, không suy ra từ độ dài nét vẽ.

| Nút | X (m) | Y (m) | Khoảng cách tới eNB |
|---|---:|---:|---|
| eNB | 0 | 0 | Mốc |
| UE1 | 20 | 0 | 20 m |
| UE2 | -10 | 17,320508 | 20 m |
| UE3 | -10 | -17,320508 | 20 m |

Tọa độ âm chỉ có nghĩa nút nằm về phía âm của trục; không phải lỗi. Cả ba UE nằm trên cùng đường tròn bán kính 20 m, đều đứng yên. Tọa độ hiển thị trong CSV được làm tròn.

## 5. Các thông số đã cài

| Thông số | Giá trị | Ý nghĩa |
|---|---|---|
| `nUe` | 3 | Số thiết bị UE |
| `dist` | 20 m | Khoảng cách mỗi UE tới eNB |
| `simTime` | 1,05 s | Thời gian ảo của mô phỏng; không phải thời gian máy tính biên dịch |
| Số eNB / cell | 1 / 1 | Ba UE được Attach trực tiếp vào một trạm; không có bài toán chọn cell hoặc handover |
| `DlBandwidth`, `UlBandwidth` | 25 RB mỗi chiều | Cấu hình tài nguyên tương ứng kênh LTE 5 MHz; không phải tốc độ 25 Mbps |
| `DlEarfcn` | 100 | Mã kênh tần số đường xuống, không phải 100 MHz |
| `UlEarfcn` | 18100 | Mã kênh tần số đường lên, không phải 18100 MHz |
| Scheduler | `PfFfMacScheduler` | Bộ lập lịch Proportional Fair; bản này chưa tạo tải ứng dụng để đánh giá việc chia tài nguyên |
| Mô hình vị trí | `ConstantPositionMobilityModel` | Các nút vô tuyến đứng yên |
| Mô hình suy hao mặc định | `FriisPropagationLossModel` | Mô hình suy hao lý tưởng của cấu hình LTE helper hiện tại |
| Ghép sóng mang | Không bật | Một sóng mang trong cấu hình này |
| S1-U, S5, S11 mặc định | 10 Gb/s; trễ lan truyền 0 s | Các liên kết có dây lý tưởng trong helper; không đại diện mạng thương mại |

RB là khối tài nguyên vô tuyến. 25 RB chiếm 4,5 MHz phần sóng mang con hữu ích và nằm trong kênh danh định 5 MHz gồm phần bảo vệ. Không cộng băng thông đường lên và xuống thành tốc độ người dùng.

## 6. Kết quả thực tế đã đo

| Chỉ tiêu | Kết quả |
|---|---:|
| Số luồng IP được FlowMonitor phân loại | 4 |
| Tổng gói gửi | 12 |
| Tổng gói nhận | 12 |
| Tổng byte IP gửi / nhận | 1.860 / 1.860 byte |
| `lostPackets` được báo cáo | 0 |
| Tổng trễ của 12 gói nhận | 1.905 ns |
| Trễ trung bình có trọng số theo số gói | 158,75 ns = 0,00015875 ms |
| Thời điểm nhận gói cuối | 0,020215053 s, khoảng 20,215 ms |

Đơn vị trên là **nanosecond (ns)**. Không đọc 158,75 ns thành 158,75 ms. Độ trễ rất nhỏ vì đang đo liên kết mạng lõi lý tưởng, không phải độ trễ từ cảm biến qua sóng LTE tới máy chủ Internet.

| Flow ID | Chiều thực tế | Gửi / nhận | Byte gửi / nhận | Trễ trung bình |
|---|---|---:|---:|---:|
| 1 | MME (13.0.0.5) → S-GW (13.0.0.6) | 3 / 3 | 492 / 492 | 266 ns |
| 2 | S-GW (14.0.0.6) → P-GW (14.0.0.5) | 3 / 3 | 492 / 492 | 133 ns |
| 3 | P-GW (14.0.0.5) → S-GW (14.0.0.6) | 3 / 3 | 438 / 438 | 118 ns |
| 4 | S-GW (13.0.0.6) → MME (13.0.0.5) | 3 / 3 | 438 / 438 | 118 ns |

Cả bốn luồng có `protocol=17` (UDP) và cổng 2123 (GTP-C). Địa chỉ mạng lõi 13.x/14.x cho thấy đây không phải các luồng ứng dụng từ UE 7.0.0.2–4. Một node có nhiều IP vì có nhiều giao diện; S-GW xuất hiện với các địa chỉ khác nhau là bình thường.

FlowMonitor phân biệt luồng bằng bộ 5 thông tin: IP nguồn, IP đích, cổng nguồn, cổng đích, giao thức. Ba UE cùng tạo thủ tục đi qua các cặp địa chỉ/cổng mạng lõi này, nên có 4 luồng chứ không phải 12 luồng.

## 7. Giải thích các trường thống kê

| Trường | Đọc như thế nào? |
|---|---|
| `flowId` | Mã luồng, không phải ID thiết bị |
| `sourceAddress`, `destinationAddress` | IP nguồn và đích của luồng được đo |
| `sourcePort`, `destinationPort` | Cổng nguồn/đích; ở đây 2123 là báo hiệu GTP-C |
| `protocol` | Số hiệu giao thức IP; 17 là UDP |
| `txPackets`, `rxPackets` | Số gói gửi và nhận trong luồng |
| `txBytes`, `rxBytes` | Số byte tầng IP, gồm IP header; không chỉ riêng nội dung ứng dụng |
| `delaySum` | Tổng trễ của tất cả gói đã nhận trong luồng |
| `minDelay`, `maxDelay` | Trễ nhỏ nhất/lớn nhất trong các gói đã nhận |
| `meanDelay_ns` trong CSV | `delaySum_ns / rxPackets`; nếu chưa nhận gói thì là NA |
| `lostPackets` | Số gói FlowMonitor đã xác định là mất; khác với mọi gói chưa tới tại lúc dừng |
| `lastRx_s` trong CSV | Thời điểm nhận gói cuối kể từ đầu mô phỏng |

Ví dụ flow 1: `delaySum=798 ns`, `rxPackets=3`, trung bình `798/3=266 ns`; min 133 ns, max 399 ns. Cả ba thủ tục có thể xếp hàng trên liên kết nên trễ các gói không nhất thiết bằng nhau.

Với bản này tx=rx nên không có gói đang chờ nhận khi dừng. Với bài khác, đừng tự kết luận `tx-rx` là mất gói vĩnh viễn nếu chưa cho mạng thời gian xử lý hết hàng đợi.

## 8. Vì sao chạy 1,05 giây mà mũi tên chỉ xuất hiện rất ngắn?

Gói cuối xuất hiện khoảng 0,020215 giây. Sau khi xong thủ tục gia nhập, bản này không có ứng dụng tạo gói mới. Vì vậy phần còn lại của mô phỏng hầu như không có hoạt động IP để xem.

- Số màu đen cạnh **Sim time** là thời gian ảo hiện tại, đơn vị giây.
- **Pause At** là thời điểm NetAnim tạm dừng phát; không quyết định thời lượng mô phỏng đã ghi trong XML.
- Thanh **fast/slow** chỉ điều chỉnh tốc độ phát lại.
- Nút **Node Size** chỉ đổi kích thước chấm vẽ, không đổi công suất, vùng phủ hay kích thước thiết bị thật.
- Các số trên lưới là tọa độ, không phải chất lượng sóng hoặc tốc độ mạng.
- **Simulation Completed** là đã phát hết dữ liệu có trong trace; không đồng nghĩa đã đánh giá mọi chức năng LTE.

## 9. Đoạn thuyết trình mẫu

“Mô hình có ba thiết bị LTE màu xanh lá, cùng kết nối tới một trạm gốc màu xanh dương. Mạng lõi gồm MME quản lý kết nối, S-GW và P-GW là hai gateway. Bài này thể hiện quá trình gia nhập mạng. Các mũi tên chúng ta thấy chủ yếu là báo hiệu giữa các thành phần mạng lõi. Kết quả ghi 12 gói gửi, 12 gói nhận, chia thành 4 luồng UDP cổng 2123. Chúng em chưa cài ứng dụng gửi số đo, nên không dùng những con số này để kết luận tốc độ Internet, độ trễ cảm biến hoặc thời lượng pin NB-IoT.”
