Họ tên sinh viên : Võ Văn Quân
MSSV : 1712698
email : quan.vovan0209@gmail.com
sdt : 0932442811

- Phần mềm sử dụng : Visual studio 2019
- Phiên bản Opencv : OpenCV 4.1.1, link tải *.dll : https://opencv.org/opencv-4-1-1/
- Ngôn ngữ : C++

- Bài nộp bao gồm : 
	+ Source code : sử dụng trình biên dịch visual studio 2019
	+ Release : chương trình được build dưới dạng release, thư mục này chưa file *.exe và image
	+ Docs : chứa báo cáo
- Hướng dẫn chạy chương trình : 
	+ chạy cmd trong thư mục release rồi thực hiện các cú pháp sau :
		BT01.exe Lena.png --rgb2gray
		BT01.exe lena_gray.png --gray2rgb
		BT01.exe Lena.png --bright 100
		BT01.exe Lena.png --contrast 3 
		
	+ Kết quả sẽ được báo cáo trực quan trong file report.pdf trong Docs