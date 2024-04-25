# **PACMAN_BY_NPC**
*Nguyễn Phú Cường - Dự án bài tập lớn môn lập trình nâng cao*

Đây là dự án cho bài tập lớn môn lập trình nâng cao, trong dự án này, mình lập trình game Pacman bằng ngôn ngữ [C++](https://en.wikipedia.org/wiki/C++), sử dụng thư viện [SDL 2.0](https://www.libsdl.org/download-2.0.php).

Hiện tại thì việc push code lên github của mình đang gặp một số lỗi, mình chưa thể up code lên đã được nửa tháng nên trong quá trình đó sẽ không có bất cứ cập nhật gì.

Big Update (3:14 26/4/2024): Anh mentor đã giúp mình push được code lên github, thay đổi ở đây đó là tạo folder .ignore bỏ qua những phần không cần push, tạo branch mới và bằng cách nào đó push được code lên thì mình cũng không hiểu lắm.

Nhìn chung thì may mắn mình vẫn push được code trước deadline, 10 điểm không có nhưng cho anh Nguyễn Vũ Thanh Tùng.

Mình học về thư viện SDL2 ở kênh youtube [phattrienphanmem](https://www.youtube.com/@PhatTrienPhanMem123AZ), nếu muốn học về thư viện SDL 2.0, bạn có thể tham khảo trang web này và trang web [SDL Wiki](https://wiki.libsdl.org/SDL2/FrontPage).

Theo dõi tiến trình phát triển dự án cùng mình tại: 

Link video: 

## MỤC LỤC

## NỘI DUNG

1. ### Cách tải thư viện SDL 2.0, cài đặt thư viện để có thể code game.

Phần này mình chia sẻ dựa trên kinh nghiệm có được từ dự án đầu tiên liên quan đến thư viện SDL 2.0, nếu không muốn đọc phần này, các bạn có thể chuyển đến phần sau ngay.
   
 Đầu tiên chúng ta cần tải thư viện SDL 2.0, ta truy cập vào [đường link này](https://www.libsdl.org/download-2.0.php) và tải đồng thời 4 thư viện:
 - [SDL 2.0](https://www.libsdl.org/download-2.0.php)
 - [SDL_image](https://www.libsdl.org/projects/SDL_image/)  
 - [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/)  
 - [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)

Bốn công cụ dùng để lập trình game SDL 2.0 mà mình thấy khá phổ biến đó là Code::Blocks, Dev C++, Visual Studio Code, Visual Studio. Ở đây thì mình sử dụng [Visual Studio Community 2022](https://visualstudio.microsoft.com/fr/downloads/).

Để cấu hình thư viện SDL 2.0 trong Visual Studio, đầu tiên ta cần giải nén các file zip đã được tải xuống, sau đó thêm vào một thư mục chung để dễ dàng cho việc di chuyển, copy tất cả các file có đuôi `.dll` vào một thư mục riêng. Theo mình được học thì có 2 cách để project của chúng ta có thể nhận được thư viện, mình dùng cách sao chép thư mục chung chứa thư viện tới vị trí cùng cấp với file có đuôi `.sln`. Sau lần build đầu tiên, nếu đã sinh ra thư mục Debug thì ra sao chép các file ở trong folder dll ta đã tạo lúc trước, dán vào thư mục Debug, nếu tồn tại thư mục bên trong thì ta phải dán các file đã sao chép vào vị trí cùng cấp với file có đuôi `.exe` nằm ở trong thư mục Debug.

Tiếp theo là phần để nhận biết thư viện ở trong Visual Studio 2022. Ta chọn vào properties của project, sau đó thiết lập đường dẫn thư mục include, lib của từng thư viện. Phần này mình sẽ không nói kĩ hơn, nếu cần bạn có thể xem tại [cấu hình thư viện SDL 2.0](https://phattrienphanmem123az.com/lap-trinh-game-c-p2/game-cpp-phan-2-cai-dat-project.html).

2. ### Cách cài đặt và chơi game.

Thao tác này rất đơn giản, bạn chỉ cần truy cập đến file zip mình đã tạo sau đó tại xuống. Khi đã tải xuống xong, bạn giải nén ở bất kì vị trí nào trong máy sau đó tìm đến file có đuôi `.exe`. Mọi việc đã xong, giờ công việc tiếp theo là chơi game.

3. ### Hướng dẫn chơi game

Trong trò chơi này, bạn trong vai một `Pacman`.

Bạn sẽ phải chạy khỏi những con `Ghost` đang săn lùng bạn, phải luôn chạy, đừng dừng chân, bởi vì bọn chúng biết phối hợp với nhau.

Bạn sẽ qua một màn khi mà bạn ăn hết tất cả các `dot` trong map.

Trong mỗi màn sẽ có 4 `power dot`, cố gấng ăn chúng, bạn sẽ được săn lùng những con ghost trong một khoảng thời gian ngắn.

Trong mỗi màn cũng sẽ xuất hiện một loại `fruit`, tìm đến và ăn chúng, sẽ được cộng khá nhiều điểm đấy.

Mỗi lượt chơi bạn sẽ có 3 mạng, cố gắng chắt chiu từng mạng và giành được điểm số cao.

Tiếp theo ta sẽ nói đến những con `Ghost`

Giống như game gốc, ở đây cũng sẽ có 4 con Ghost với các cách di chuyển khác nhau để chúng về cơ bản có thể phối hợp với nhau, lần lượt đó là:

- `Blinky`: Kẻ nóng tính nhất, luôn đuổi theo Pacman.
- `Pinky`: Làm chim mồi, đi đến vị trí trước mặt Pacman 4 ô, nếu không đạt được mục đích, lập tức đuổi theo Pacman.
- `Inky`: Kẻ mưu trí nhất, đi đến vị trí đôi xứng với Blinky qua Pacman, nói cách khác chính là đón đầu Pacman, nó cũng sẽ đuổi theo Pacman nếu không đạt được mục đích.
- `Clyde`: Đánh du kích, đi ngẫu nhiên quanh rìa bản đồ, khi phát hiện Pacman còn cách mình dưới 8 ô thì đuổi theo Pacman.

Ghost được chia ra làm 3 trạng thái:

- `Thăm dò`: Khi ở chế độ thăm dò, Ghost sẽ di chuyển ở một góc trên bản đồ trong một khoảng thời gian ngắn. Ở trong game gốc thì chế độ thăm dò sẽ được áp dụng với tất cả con Ghost, nhưng mình sẽ chỉ dùng chế độ thăm dò cho Blinky, thay vào đó sẽ set thời gian các Ghost khác ra khỏi lồng là lâu hơn.
- `Săn lùng`: Trong chế độ săn lùng, các Ghost sẽ phối hợp với nhau để ăn được Pacman, có thể bạn sẽ nghĩ các ghost sẽ di chuyển như nhau, tuy nhiên mỗi ghost sẽ có một mục tiêu khác nhau.
-   `Hoảng sợ`: Chế độ hoảng sợ của Ghost bắt đầu khi Pacman ăn được power dot. Khi đó Ghost sẽ không đuổi theo Pacman, nếu bị Pacman ăn thì sẽ chuyển thành `đôi mắt`, sau đó tìm đường đi ngắn nhất về cửa lồng để khôi phục trạng thái bình thường.

Trong bản đồ sẽ có những hình khối sau:
-
-
-

4. ### Thuật toán của game

Bản đồ mình quản lí bằng các ô, mỗi ô có kích thước là 30x30 pixel. Ban đầu mình muốn quản lí map bằng pixel, nhưng sau một tuần không thể tìm ra được giải pháp nào hợp lí, cũng không thấy có kênh hay trang nào làm bằng cách này thì mình đã chuyển đổi sang ô. Việc kiểm tra va chạm map với nhân vật cũng khá đơn giản, lưu ý lớn nhất là thay đổi và kiểm tra các trường hợp sao cho nhân vật không bị out ra khỏi map, bởi vì kích thước đường đi đúng bằng kích thước map nên rất dễ xảy ra sai số.

Trước tiên ta sẽ nói về sự di chuyển của Pacman

Pacman của mình có kích thước 30x30, bằng với kích thước của đường đi trong bản đồ, vì thế có một vấn đề xảy ra đó chính là khi di chuyển nếu muốn đổi hướng thì phải vào đúng chính xác giữa ô thì mới có thể chuyển hướng được, khi di chuyển thì khá khó chịu.

Sau hơn 1 tuần tìm kiếm giải pháp thì mình đã nghĩ ra một cách. Ban đầu mình quản lí di chuyển bằng một biến là `pacman_status_`, sau đó mình thêm một biến quản lí di chuyển nữa là `arrrow_status_`. Vậy mình quản lí chúng như thế nào? Bản chất thì `arrow_status_` mình sẽ dùng để quản lí lệnh di chuyển trực tiếp, có nghĩa là mình bấm vào bàn phím thì biến này sẽ được cập nhật. Còn `pacman_status_` thì mình sẽ quản lí lệnh di chuyển thứ 2 cần xử lí. Ta đã biết vai trò của 2 biến, giờ ta thực hiện di chuyển bằng cách kiểm tra xem lệnh di chuyển có thực hiện được không, tương đương với việc có bị chạm vào tường hay không, nếu di chuyển được thì ta thực hiện với `arrrow_status_`, ngược lại ta xử lí với `pacman_status_`. Mình cũng đã tạo thêm mũi tên ở bên ngoài Pacman, hướng của mũi tên sẽ là trạng thái của `arrrow_status_`, còn hướng của Pacman sẽ là trạng thái của `pacman_status_`. Xử lí xong tất cả điều này, việc di chuyển và xác định hướng di chuyển đã dễ dàng hơn rất nhiều.

Tiếp theo mình xét đến Ghost

Khác với game gốc, ở đây mình sẽ thực hiện việc di chuyển của Ghost bằng thuật toán BFS. Khi tải hình ảnh và gắn cho nó chuyển động một chiều thì khá đơn giản, mọi thứ chỉ cần làm đúng logic thì đã di chuyển mượt mà. Nhưng sau khi ta gắn thuật toán BFS cho Ghost thì đã xuất hiện nhiều hiện tượng lạ như nhiễu chuyển động, cụ thể là Ghost sẽ giật theo nhiều hướng mỗi khi phải chuyển hướng và bị đi xuyên qua map. Ban đầu mình định để cho việc kiểm tra va chạm với map ta không cần hàm kiểm tra như Pacman nữa, mà ta sẽ kiểm tra luôn bằng thuật toán BFS. Nhưng sau đó mình nhận ra có quá nhiều vấn đề với sự thay đổi này, nên mình đã quay lại với hàm kiểm tra va chạm với bản đồ. Sau đó chính hàm đó là hàm chữa lỗi cho sự di chuyển không nhất quán mà hàm BFS tạo nên, mình tạo ra một con số được gọi là sai số, rồi kiểm tra từng trường hợp, ban đầu mình không hi vọng lắm vào cách làm này, nhưng may mắn là mình đã kiểm tra tất cả các trường hợp và nó đã thành công. 

Công việc kiểm tra va chạm của Ghost và Pacman thì không mấy khó khăn, mình dùng 3 cặp tọa độ x, y với mỗi nhân vật, nói như này có thể gây khó hiểu, vậy bạn hãy tưởng tượng nhân vật là một hình vuông, thì 3 điểm mình tạo là điểm nằm ở `góc trên bên trái(1)`, điểm nằm ở `góc trên bên phải(2)` và `góc dưới bên trái(3)`, sau đó mình chia trường hợp thì cụ thể nó sẽ được chia thành 4 trường hợp mà mình sẽ nói ngay sau đây. Trường hợp đầu tiên đó là Ghost đi từ bên phải Pacman, khi đó ta xét điểm (1) của Pacman với điểm (2) của Ghost, ngược lại với Ghost đi từ bên trái của Pacman thì ta xét điểm (2) của Pacman với điểm (1) của Ghost. Tiếp theo nếu Ghost đi từ dưới lên vị trí của Pacman, ta xét điểm (3) của Pacman với điểm (1) của Ghost. Cuối cùng với trường hợp Ghost đi từ trên xuống vị trí của Pacman thì ta xét điểm (1) của Pacman với điểm (3) của Ghost. Sau đó ta cũng cộng thêm một lượng sai số, mình tìm bằng cách kiểm thử từng trường hợp.

Về phần chuyển động xuyên suốt game của Ghost, mình kiểm soát nó bằng các hàm di chuyển áp dụng cho từng con Ghost. Thuật toán di chuyển cũng không quá khó hiểu khi mà ta chỉ xoay quanh việc di chuyển bằng thuật toán BFS và di chuyển ngẫu nhiên, điều khó nhất có lẽ là áp dụng thuật toán BFS gắn với chuyển động của Ghost. Vậy vấn đề còn lại đó là việc chọn ra được mục tiêu cho từng con Ghost, với mục tiêu tmình quản lí bằng ô trên bản đồ để dễ dàng kiểm tra qua lại giữa Pacman, Ghost và bản đồ, việc này thì mình triển khai như sau:

- `Blinky`: Dùng thẳng thuật toán BFS với mục tiêu là Pacman.
- `Pinky`: Ta lấy mục tiêu là ô ở trước mặt Pacman cách nó 4 ô, để lấy được ô này thì ta phải xét từng trạng thái di chuyển rồi mới lấy được mục tiêu. Sau đó ta kiểm tra xem mục tiêu có nằm ngoài bản đồ hay không, nếu nằm ngoài thì ta xét đại lượng nào nằm bên ngoài thì ta sẽ cho nó là ô ngoài cùng nhất là ô đi được. Khi kiểm tra xong ta lại kiểm tra xem ô mục tiêu có phải là tường hay không, nếu không thì mục tiêu chính là ô đó, còn nếu đúng thì gán mục tiêu bằng ô hiện tại của Pacman.
- `Inky`: 


