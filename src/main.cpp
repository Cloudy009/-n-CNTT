#include"raylib.h"
#include<string>
#include<iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <queue>
#include <thread>
#include <random>
#include <chrono> // Thêm thư viện chrono
#include <unistd.h>
#include <set> 
#define MAX 100
using namespace std;
int taoMaTran();
void docFile() ;
int A[MAX][MAX];
int originalA[MAX][MAX];
int n,m ;
void ve(int A[][MAX], int cot, int hang);
void giaoDienHuongDan();
void resetMang();
void randomMang(int A[][MAX], int cot, int hang);
void giaoDienDoThi(int A[][MAX], int cot, int hang);
void timDuongBFS(int A[][MAX], int cot, int hang);
bool kiemTraDuongDi(int A[][MAX], int cot, int hang);
void timDuongBFSTruyVet(int A[][MAX], int cot, int hang);
void timDuongDFSTruyVet(int A[][MAX], int cot, int hang, int x, int y);
void distra(int A[MAX][MAX], int m, int n);
bool HopLe(int x, int y);
void chuyenMaTranAsangke();
enum TrangThai { TRANGTHAI_CHINH, TRANGTHAI_DOTHI, TRANGTHAI_HUONGDAN };

template <typename T>
struct Dinh 
{
	string ten;
	T thongTin;

	Dinh(const string& tenDinh, const T& thongTinDinh) : ten(tenDinh), thongTin(thongTinDinh) {}
};

// Định nghĩa kiểu dữ liệu cho cạnh với trọng số
template <typename W>
struct Canh 
{
	int dinhXuatPhat;
	int dinhKetThuc;
	W trongSo;

	Canh(int xuatPhat, int ketThuc, const W& trongSoCanh) : dinhXuatPhat(xuatPhat), dinhKetThuc(ketThuc), trongSo(trongSoCanh) {}
};

template <typename T, typename W>
class DoThi 
{
	private:
		vector<Dinh<T>> dinh;
		vector<Canh<W>> canh;
		vector<vector<W>> maTranTrongSo;
	public:
		// Tạo một đồ thị mới
		DoThi() {}

		// Thêm một đỉnh vào đồ thị
		void ThemDinh(const string& tenDinh, const T& thongTinDinh) 
		{
			Dinh<T> motDinh(tenDinh, thongTinDinh);
			dinh.push_back(motDinh);
		}

		// Thay đổi thông tin của một đỉnh
		void ThayDoiThongTinDinh(int viTri, const T& thongTinMoi) 
		{
			if (viTri >= 0 && viTri < dinh.size()) {
				dinh[viTri].thongTin = thongTinMoi;
			}
		}

		// Thêm một cạnh với trọng số
		void ThemCanh(int xuatPhat, int ketThuc, const W& trongSo) 
		{
			if (xuatPhat >= 0 && xuatPhat < dinh.size() && ketThuc >= 0 && ketThuc < dinh.size()) 
			{
				Canh<W> motCanh(xuatPhat, ketThuc, trongSo);
				canh.push_back(motCanh);
			}
		}

		// Thay đổi trọng số của cạnh
		void ThayDoiTrongSoCanh(int xuatPhat, int ketThuc, const W& trongSoMoi) 
		{
			for (Canh<W>& motCanh : canh) 
			{
				if (motCanh.dinhXuatPhat == xuatPhat && motCanh.dinhKetThuc == ketThuc) 
				{
					motCanh.trongSo = trongSoMoi;
					break;
				}
			}
			A[xuatPhat][ketThuc] = trongSoMoi;
    		A[ketThuc][xuatPhat] = trongSoMoi;
		}

		// Xuất tên của các đỉnh
		void XuatTenDinh() 
		{
			for (const Dinh<T>& motDinh : dinh) 
			{
				cout << motDinh.ten << " ";
			}
			cout << endl;
		}

		// Xuất thông tin của một đỉnh
		void XuatThongTinDinh(int viTri) 
		{
			if (viTri >= 0 && viTri < dinh.size()) 
			{
				cout << "Ten dinh: " << dinh[viTri].ten << endl;
				cout << "Thong tin dinh: " << dinh[viTri].thongTin << endl;
			}
		}

		// Xuất thông tin của một cạnh
		void XuatThongTinCanh(int xuatPhat, int ketThuc) 
		{
			for (const Canh<W>& motCanh : canh) 
			{
				if (motCanh.dinhXuatPhat == xuatPhat && motCanh.dinhKetThuc == ketThuc) 
				{
					cout << "Trong so canh (" << dinh[xuatPhat].ten << ", " << dinh[ketThuc].ten << "): " << motCanh.trongSo << endl;
					break;
				}
			}
		}

		void TaoMaTranTrongSo() 
		{
			maTranTrongSo.resize(dinh.size(), vector<W>(dinh.size(), W())); // Khởi tạo ma trận trọng số

			for (const Canh<W>& motCanh : canh) 
			{
				maTranTrongSo[motCanh.dinhXuatPhat][motCanh.dinhKetThuc] = motCanh.trongSo;
				maTranTrongSo[motCanh.dinhKetThuc][motCanh.dinhXuatPhat] = motCanh.trongSo; // Đồ thị vô hướng
			}
			for (int i = 0; i < dinh.size(); ++i) 
   			{
        		for (int j = 0; j < dinh.size(); ++j) 
        		{
            		A[i][j] = maTranTrongSo[i][j];
        		}
    		}
    		
    		n = dinh.size(); // Lưu số hàng (n)
    		m = dinh.size(); // Lưu số cột (m)
			A[n-1][m-1] = 1;
			A[0][0] = 1;

   			 // Xuất số hàng và số cột
		}

		// Xuất ma trận trọng số
		void XuatMaTranTrongSo() 
		{
			for (const vector<W>& hang : maTranTrongSo) 
			{
				for (const W& trongSo : hang) {
					cout << trongSo << " ";
				}
				cout << endl;
			}
		}

		void XuatMaTranKe() 
		{
			for (int i = 0; i < dinh.size(); ++i) 
			{
				for (int j = 0; j < dinh.size(); ++j) 
				{
					if (maTranTrongSo[i][j] != W()) 
					{
						cout << "1 ";
					} 
					else 
					{
						cout << "0 ";
					}
				}
				cout << endl;
			}
		}

		// Xuất ma trận liên kết
		void XuatMaTranLienKet() 
		{
			for (int i = 0; i < dinh.size(); ++i) 
			{
				for (int j = 0; j < dinh.size(); ++j) 
				{
					if (maTranTrongSo[i][j] != W() || i == j) 
					{
						cout << "1 ";
					} 
					else 
					{
						cout << "0 ";
					}
				}
				cout << endl;
			}
		}
		// Duyệt đồ thị theo chiều rộng (BFS)
		void DuyetBFS(int dinhBatDau) 
		{
			vector<bool> daTham(dinh.size(), false); // Đánh dấu các đỉnh đã thăm
			queue<int> hangDoi; // Queue để duyệt BFS

			daTham[dinhBatDau] = true;
			hangDoi.push(dinhBatDau);

			while (!hangDoi.empty()) 
			{
				int dinhHienTai = hangDoi.front();
				cout << dinh[dinhHienTai].ten << " ";
				hangDoi.pop();

				for (int i = 0; i < dinh.size(); ++i) 
				{
					if (maTranTrongSo[dinhHienTai][i] != W() && !daTham[i]) 
					{
						daTham[i] = true;
						hangDoi.push(i);
					}
				}
			}
		}

		// Duyệt đồ thị theo chiều sâu (DFS)
		void DuyetDFS(int dinhBatDau, vector<bool>& daTham) 
		{
			daTham[dinhBatDau] = true;
			cout << dinh[dinhBatDau].ten << " ";

			for (int i = 0; i < dinh.size(); ++i) 
			{
				if (maTranTrongSo[dinhBatDau][i] != W() && !daTham[i]) 
				{
					DuyetDFS(i, daTham);
				}
			}
		}

		void DuyetDFSChieuSau(int dinhBatDau) 
		{
			vector<bool> daTham(dinh.size(), false); // Đánh dấu các đỉnh đã thăm
			DuyetDFS(dinhBatDau, daTham);
		}	
		
		void TimDuongNganNhatDijkstra(int dinhXuatPhat) 
		{
	        vector<W> khoangCach(dinh.size(), numeric_limits<W>::max()); // Khởi tạo khoảng cách từ đỉnh xuất phát đến các đỉnh khác
	        set<pair<W, int>> hangDoi; // Queue ưu tiên lưu (khoảng cách, đỉnh)
	
	        khoangCach[dinhXuatPhat] = 0;
	        hangDoi.insert({0, dinhXuatPhat});
	
	        while (!hangDoi.empty()) 
			{
	            int dinhHienTai = hangDoi.begin()->second;
	            hangDoi.erase(hangDoi.begin());
	
	            for (int i = 0; i < dinh.size(); ++i) 
				{
	                if (maTranTrongSo[dinhHienTai][i] != W()) 
					{
	                    W trongSo = maTranTrongSo[dinhHienTai][i];
	                    if (khoangCach[dinhHienTai] + trongSo < khoangCach[i]) 
						{
	                        hangDoi.erase({khoangCach[i], i});
	                        khoangCach[i] = khoangCach[dinhHienTai] + trongSo;
	                        hangDoi.insert({khoangCach[i], i});
	                    }
	                }
	            }
        	}

	        cout << "Khoang cach tu dinh " << dinh[dinhXuatPhat].ten << " den cac dinh con lai:" << endl;
	        for (int i = 0; i < dinh.size(); ++i) 
			{
	            cout << "Tu " << dinh[dinhXuatPhat].ten << " den " << dinh[i].ten << ": " << khoangCach[i] << endl;
	        }
    	}	

		void XuatDoThi() 
		{
			cout << "Danh sach DINH:" << endl;
			for (int i = 0; i < dinh.size(); ++i) 
			{
				cout << "Dinh " << i << ": " << dinh[i].ten << " (Thong tin: " << dinh[i].thongTin << ")" << endl;
			}

			cout << "Danh sach cac canh:" << endl;
			for (int i = 0; i < canh.size(); ++i) 
			{
				cout << "Canh " << i << ": (" << dinh[canh[i].dinhXuatPhat].ten << " -> " << dinh[canh[i].dinhKetThuc].ten << ") Trong so: " << canh[i].trongSo << endl;
			}
		}
		
		bool CoChuTrinhEuler() 
		{
			for (int i = 0; i < dinh.size(); ++i) 
			{
				int bac = 0; // Đếm số cạnh kề với đỉnh i
				for (int j = 0; j < dinh.size(); ++j) 
				{
					if (maTranTrongSo[i][j] != W()) 
					{
						bac++;
					}
				}
				if (bac % 2 != 0) 
				{
					return false; // Nếu có ít hơn hoặc nhiều hơn 2 đỉnh có bậc lẻ, không có chu trình Euler
				}
			}
			return true;
		}
		
		void DatLaiTrongSo(int min, int max) 
		{
			random_device rd;  // Đối tượng để tạo giá trị ngẫu nhiên
    		mt19937 gen(rd());
    		uniform_int_distribution<int> distribution(min, max);
		    for (Canh<W>& motCanh : canh) 
		    {
		        motCanh.trongSo = distribution(gen);;
		    }
		}

    	void ThemCanhNgauNhien(int soCanh, int minTrongSo, int maxTrongSo)
		{
		    random_device rd;
		    mt19937 gen(rd());
		    uniform_int_distribution<int> distribution(minTrongSo, maxTrongSo);
		
		    int soDinh = dinh.size();
		
		    for (int i = 0; i < soCanh; i++)
		    {
		        int dinhXuatPhat = rand() % soDinh;
		        int dinhKetThuc = rand() % soDinh;
		        int trongSo = distribution(gen);
		
		        if (dinhXuatPhat != dinhKetThuc)
		        {
		            ThemCanh(dinhXuatPhat, dinhKetThuc, trongSo);
		        }
		        else
		        {
		            i--; // Nếu cạnh kết thúc và xuất phát trùng nhau, thử lại
		        }
		    }
		}
		void Prim() 
	    {
	        vector<int> parent(dinh.size(), -1); // Lưu trữ các đỉnh cha của cây con bé nhất
	        vector<W> minWeight(dinh.size(), numeric_limits<W>::max()); // Lưu trọng số nhỏ nhất của các đỉnh
	
	        // Chọn một đỉnh bất kỳ làm đỉnh xuất phát
	        int startVertex = 0;
	        minWeight[startVertex] = 0;
	
	        for (int i = 0; i < dinh.size() - 1; ++i) 
	        {
	            int u = -1;
	            for (int v = 0; v < dinh.size(); ++v) 
	            {
	                if (!minWeight[v] && (u == -1 || minWeight[v] < minWeight[u])) 
	                {
	                    u = v;
	                }
	            }
	
	            minWeight[u] = 0;
	
	            for (int v = 0; v < dinh.size(); ++v) 
	            {
	                if (maTranTrongSo[u][v] != W() && maTranTrongSo[u][v] < minWeight[v]) 
	                {
	                    parent[v] = u;
	                    minWeight[v] = maTranTrongSo[u][v];
	                }
	            }
	        }
	
	        // Xuất cây con bé nhất
	        cout << "Cay con be nhat (Minimum Spanning Tree):" << endl;
	        for (int i = 1; i < dinh.size(); ++i) 
	        {
	            cout << dinh[parent[i]].ten << " - " << dinh[i].ten << " Trong so: " << minWeight[i] << endl;
	        }
	    }
};

void XoaManHinh() 
{
	#ifdef _WIN32
    system("cls"); // Nếu đang chạy trên Windows
    #endif
}

void save() 
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            originalA[i][j] = A[i][j];
        }
    }
}

void chuyenMaTranAsangke()
{
	 for (int i = 0; i < n; i++) {
	        for (int j = 0; j < m; j++) {
	            A[i][j] = (A[i][j] != 0) ? 1 : 0; // Gán 1 nếu có cạnh, ngược lại gán 0
	        }
	    }
	
	    // Xuất ma trận liên kết (ma trận A đã được cập nhật)
	    cout << "Ma tran lien ket (ma tran A da duoc chuyen doi):" << endl;
	    for (int i = 0; i < n; i++) {
	        for (int j = 0; j < m; j++) {
	            cout << A[i][j] << " ";
	        }
	        cout << endl;
	    }
}

//void XuatMenu() 
//{
//    cout << "Menu:" << endl;
//    cout << "1. Xuat do thi " << endl;
//    cout << "2. Thong tin dinh" << endl;
//    cout << "3. Thong tin canh" << endl;
//    cout << "4. Ma tran trong so" << endl;
//    cout << "5. Ma tran ke" << endl;
//    cout << "6. Ma tran lien ket" << endl;
//    cout << "7. Kiem tra chu trinh Euler" << endl;
//    cout << "8. Duyet BFS" << endl;
//    cout << "9. Duyet DFS" << endl;
//    cout << "10. Tao lai do thi" << endl;
//    cout << "11. Tim duong di ngan nhat tu 1 dinh den cac dinh con lai" << endl;
//   	cout << "12. Xuat mang A" << endl;
//    cout << "13. Cay con nho nhat" << endl; 
//	cout << "14. Xoa man hinh" << endl;
//	cout << "15. Thoat!" << endl;
//}
//
//int main() {
//    DoThi<string, int> dothi;
//
//    // Thêm đỉnh và cạnh vào đồ thị
//    // Thêm 10 đỉnh vào đồ thị và gán thông tin
//    dothi.ThemDinh("A", "Thong tin A");
//    dothi.ThemDinh("B", "Thong tin B");
//    dothi.ThemDinh("C", "Thong tin C");
//    dothi.ThemDinh("D", "Thong tin D");
//    dothi.ThemDinh("E", "Thong tin E");
//    dothi.ThemDinh("F", "Thong tin F");
//    dothi.ThemDinh("G", "Thong tin G");
//    dothi.ThemDinh("H", "Thong tin H");
//    dothi.ThemDinh("I", "Thong tin I");
//    dothi.ThemDinh("J", "Thong tin J");
//    dothi.ThemDinh("K", "Thong tin K");
//	dothi.ThemDinh("L", "Thong tin L");
//	dothi.ThemDinh("M", "Thong tin M");
//	dothi.ThemDinh("N", "Thong tin N");
//	dothi.ThemDinh("O", "Thong tin O");
//	dothi.ThemDinh("P", "Thong tin P");
//	dothi.ThemDinh("Q", "Thong tin Q");
//	dothi.ThemDinh("R", "Thong tin R");
//	dothi.ThemDinh("S", "Thong tin S");
//	dothi.ThemDinh("T", "Thong tin T");
//
//
//    // Thêm cạnh giữa các đỉnh để tạo cấu trúc logic
//    dothi.ThemCanh(0, 1, 5);
//    dothi.ThemCanh(0, 0, 1);
//    dothi.ThemCanh(9, 9, 3);
//	dothi.ThemCanhNgauNhien(100,0,9);
//
//    int choice;
//    while (true) {
//        XuatMenu();
//        cout << "Chon chuc nang: ";
//        cin >> choice;
//
//        switch (choice) {
//        	case 1:
//        		dothi.XuatDoThi();
//        		break;
//            case 2:
//                dothi.XuatThongTinDinh(2);
//                break;
//            case 3:
//                dothi.XuatThongTinCanh(0, 2);
//                break;
//            case 4:
//                // Xóa ma trận trọng số cũ trước khi tạo mới
////                dothi.XoaMaTranTrongSo();
//                dothi.TaoMaTranTrongSo();
//                cout << "MA TRAN TRONG SO:" << endl;
//                dothi.XuatMaTranTrongSo();
//                cout << endl;
//                break;
//            case 5:
//                cout << "MA TRAN KE:" << endl;
//                dothi.XuatMaTranKe();
//                cout << endl;
//                break;
//            case 6:
//                cout << "MA TRAN LIEN KET:" << endl;
//                dothi.XuatMaTranLienKet();
//                cout << endl;
//                break;
//            case 7:
//                if (dothi.CoChuTrinhEuler()) {
//                    cout << "Do thi co chu trinh EULER." << endl;
//                } else {
//                    cout << "Do thi KHONG CO chu trinh EULER." << endl;
//                }
//                break;
//            case 8:
//                cout << "Duyet do thi theo chieu RONG (BFS):" << endl;
//                dothi.DuyetBFS(0);
//                cout << endl;
//                break;
//            case 9:
//                cout << "Duyet do thi theo chieu SAU (DFS):" << endl;
//                dothi.DuyetDFSChieuSau(0);
//                cout << endl;
//                break;
//            case 10:
////                dothi.XoaMaTranTrongSo();
////                dothi.TaoMaTranTrongSo();
//                dothi.DatLaiTrongSo(0,9);
//                break;
//            case 11:
//            	// Tìm đường đi ngắn nhất từ đỉnh 0 (A) đến tất cả các đỉnh còn lại
//    			dothi.TimDuongNganNhatDijkstra(0);
//    			break;
//            case 12:
//            	cout << "So hang (n): " << n << endl;
//    			cout << "So cot (m): " << m << endl;
//            	for( int i=0; i<n; i++)
//            	{
//					for(int j=0; j < m; j++)
//            		{
//						cout<<A[i][j]<<" ";
//					}
//					cout<<endl;
//				}
//				break;
//			case 13:
//				dothi.Prim();
//				break;
//			case 14:
//                XoaManHinh();
//                XuatMenu();
//                cin.ignore();
//                cout << "Nhan Enter de tiep tuc...";
//                cin.get();
//                break;
//            case 15:
//                return 0;
//            case 16:
//            	chuyenMaTranAsangke();
//            	break;
//            default:
//                cout << "Chức năng không hợp lệ. Vui lòng chọn lại." << endl;
//                break;
//        }
//    }
//    return 0;
//}

int main() 
{
	DoThi<string, int> dothi;

    // Thêm đỉnh và cạnh vào đồ thị
    // Thêm 10 đỉnh vào đồ thị và gán thông tin
    dothi.ThemDinh("A", "Thong tin A");
    dothi.ThemDinh("B", "Thong tin B");
    dothi.ThemDinh("C", "Thong tin C");
    dothi.ThemDinh("D", "Thong tin D");
    dothi.ThemDinh("E", "Thong tin E");
    dothi.ThemDinh("F", "Thong tin F");
    dothi.ThemDinh("G", "Thong tin G");
    dothi.ThemDinh("H", "Thong tin H");
    dothi.ThemDinh("I", "Thong tin I");
    dothi.ThemDinh("J", "Thong tin J");
    dothi.ThemDinh("K", "Thong tin K");
	dothi.ThemDinh("L", "Thong tin L");
	dothi.ThemDinh("M", "Thong tin M");
	dothi.ThemDinh("N", "Thong tin N");
	dothi.ThemDinh("O", "Thong tin O");
	dothi.ThemDinh("P", "Thong tin P");
	dothi.ThemDinh("Q", "Thong tin Q");
	dothi.ThemDinh("R", "Thong tin R");
	dothi.ThemDinh("S", "Thong tin S");
	dothi.ThemDinh("T", "Thong tin T");


    // Thêm cạnh giữa các đỉnh để tạo cấu trúc logic
    dothi.ThemCanh(0, 1, 5);
    dothi.ThemCanh(0, 0, 1);
    dothi.ThemCanh(9, 9, 3);
	dothi.ThemCanhNgauNhien(600,0,9);

	dothi.TaoMaTranTrongSo();	
	chuyenMaTranAsangke();
	save(); 
//	taoMaTran();
//	docFile() ;

	int hang = n;
	int cot = m;
	
    int cao = 900;
    int rong = 900;		
    
    TrangThai trangThai = TRANGTHAI_CHINH; //đặt trạng thái ban đầu
    
	InitWindow(rong, cao, "Trò chơi mê cung");
	
	InitAudioDevice();   
    
	Image logo = LoadImage("taiNguyen/image/logo.png");//tạo biến ảnh
    SetWindowIcon(logo); //load lên logo cửa sổ 
    
	Texture2D hinhNen = LoadTexture("taiNguyen/image/test.png");
	Texture2D hinhHuongDan = LoadTexture("taiNguyen/image/loiVao2.png");
	Texture2D gameName = LoadTexture("taiNguyen/image/gameName.png");
	Texture2D turnOn = LoadTexture("taiNguyen/image/turnOn.png");
	Texture2D turnOff = LoadTexture("taiNguyen/image/turnOff.png");

    Rectangle nutBatDau = {630, 650 , 180, 60};
 	Rectangle quayLai = {750, 830, 100, 40};
	Rectangle huongDan = {50, 650, 180, 60};
	Rectangle exit = {750,800, 100,40};
	Rectangle soundClick = {10,70, 100,40};
	
	Rectangle turnMusic = {10, 10, 100, 60};

	bool offSound = false;
	bool sound = true;
	bool offMusic = true; 
	bool pause = false;
	bool choPhepClick = false;
	Sound clickSound = LoadSound("E:/hoc/App/projectC++/FolderC++/thuVienAnh/clickSound.wav");
	
	Music gameMusic = LoadMusicStream("taiNguyen/sound/gameMusic.mp3");
	PlayMusicStream(gameMusic);
	
	SetTargetFPS(60);
	
    while (!WindowShouldClose()) 
    {	
    	UpdateMusicStream(gameMusic);

    	if( IsKeyPressed(KEY_SPACE))
    	{
    		pause = !pause;
    		if(pause)
    		{
    			PauseMusicStream(gameMusic);
			}
			else
			{
				ResumeMusicStream(gameMusic);
			}
		}
    	
    	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and sound == true)
		{
			choPhepClick = true; 
			PlaySound(clickSound);
		}
		
        BeginDrawing();
        ClearBackground(WHITE);
		
		
        if (trangThai == TRANGTHAI_CHINH)
        {
            // Hiển thị giao diện chính 
			DrawTexture(hinhNen, 0, 0, WHITE);
			
			// Hiển thị tên game
			DrawTexture(gameName, 150, -100, WHITE);
			
			DrawText("GUIDE", huongDan.x + 30, huongDan.y + 17, 40, WHITE);
			
            DrawText("START", nutBatDau.x + 23, nutBatDau.y + 17, 40 , WHITE);
            
			if(offMusic)
			{
				DrawTexture(turnOn, turnMusic.x + 23, turnMusic.y + 17, WHITE);
			}
			else
			{
				DrawTexture(turnOff, turnMusic.x + 23, turnMusic.y + 17, WHITE);
			}
			
			if(offSound) DrawText("off click", soundClick.x + 23, soundClick.y + 17, 20, WHITE);
			else DrawText("On click", soundClick.x + 23, soundClick.y + 17, 20, WHITE);
			
			if (CheckCollisionPointRec(GetMousePosition(), soundClick) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
			{
				sound = !sound;
				if(sound)
				{
					offSound = false;
				}
				else
				{
					PlaySound(clickSound);
					offSound = true;
				}
			}
			
			
			if (CheckCollisionPointRec(GetMousePosition(), turnMusic) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
            {
//				UpdateMusicStream(gameMusic);
               	pause = !pause;
               	if(pause)
               	{
               		StopMusicStream(gameMusic);
               		offMusic = false;
			   	}
			   	else
			   	{	
			   		PlayMusicStream(gameMusic);
			   		offMusic = true;
			   	}
            }
			
            if ( (CheckCollisionPointRec(GetMousePosition(), nutBatDau) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )|| (IsKeyPressed(KEY_ENTER))) 
            {
                // Chuyển sang trạng thái giao diện đồ thị khi nút "Bắt đầu" được nhấn
                trangThai = TRANGTHAI_DOTHI;
//                taoMaTran();
//				docFile();
				dothi.DatLaiTrongSo(0,9);
				chuyenMaTranAsangke();
				save();
				
            }
            if (CheckCollisionPointRec(GetMousePosition(), huongDan) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
            {
                // Chuyển sang trạng thái giao diện đồ thị khi nút "Bắt đầu" được nhấn
                trangThai = TRANGTHAI_HUONGDAN;
            }
        }
        else if (trangThai == TRANGTHAI_DOTHI)
        {
            // Hiển thị giao diện đồ thị
			ClearBackground(WHITE);
			
			DrawRectangle(800,0, 100, 800, BLACK);
			DrawRectangle(801,1, 98, 798, WHITE);
			DrawRectangle(0,800, 900, 99, BLACK);
            DrawRectangle(0,800, 900, 99, BLACK);
            DrawRectangle(1,801, 898, 97, WHITE);
            
		    DrawRectangleRec(quayLai, RED);	//màu button
		    DrawText("EXIT", quayLai.x + 23, quayLai.y + 17, 20, WHITE);//text cho button
            ve(A, cot, hang);
            giaoDienDoThi(A, cot, hang);
            
            if (CheckCollisionPointRec(GetMousePosition(),quayLai) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
            {
                // Chuyển sang trạng thái giao diện đồ thị khi nút "Bắt đầu" được nhấn
                trangThai = TRANGTHAI_CHINH;
            }
        }
        else if( trangThai == TRANGTHAI_HUONGDAN )
        {
        	DrawTexture(hinhHuongDan, 0, 0, WHITE);
        	DrawRectangleRec(exit, RED);	//màu button
		    DrawText("EXIT", exit.x + 23, exit.y + 15, 20, WHITE);//text cho button
        	giaoDienHuongDan();
        	 if (CheckCollisionPointRec(GetMousePosition(),exit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
            {
                // Chuyển sang trạng thái giao diện đồ thị khi nút "Bắt đầu" được nhấn
                trangThai = TRANGTHAI_CHINH;
            }
		}
		
		
		if(choPhepClick)
		{
			DrawCircleV(GetMousePosition(), 20 , WHITE);
			PlaySound(clickSound);
		}
		choPhepClick = false;
		
        EndDrawing();
    }

	UnloadMusicStream(gameMusic);   // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    UnloadTexture(hinhNen); 
    CloseWindow();
    return 0;
    cout<<"END!!";
}

int taoMaTran()
{
   srand(static_cast<unsigned int>(time(nullptr)));

    // Kích thước của ma trận hai chiều
    int rows = 20; // Đổi giá trị rows theo ý muốn
    int cols = 20; // Đổi giá trị cols theo ý muốn

    // Tạo mảng hai chiều và điền giá trị ngẫu nhiên (0 hoặc 1)
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) 
		{
			if (i%2 == 0)
				matrix[i][j] = 1;
//			else if(j%2 == 1 and i%2 == 1)
//				matrix[i][j] = 1;
			else
            	matrix[i][j] = rand() % 2;
        }
    }
	
	
    // Kiểm tra số lượng giá trị 1 và 0
    int count1 = 0;
    int count0 = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (matrix[i][j] == 1) {
                count1++;
            } else {
                count0++;
            }
        }
    }

    // Nếu số lượng giá trị 1 không đủ lớn, tạo lại ma trận
    if (count1 <= 2 * count0) {
        cout << "Matrix does not meet the condition. Recreating..." << endl;
        for (int i = 0; i < rows; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
        return taoMaTran(); // Gọi lại hàm tạo ma trận
    }
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            originalA[i][j] = matrix[i][j];
        }
    }

    // Mở file để ghi ma trận vào
    ofstream outputFile("maTran.txt");

    // Kiểm tra xem file có mở thành công không
    if (!outputFile.is_open()) {
        cerr << "Can't open maTran.txt for recording!" << endl;
        return 1;
    }
    outputFile << rows << " " << cols << "\n";
	
    // Ghi ma trận vào file
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            outputFile << matrix[i][j] << " ";
        }
        outputFile << "\n"; // Xuống dòng sau mỗi hàng
    }
    
    cout << "Create a success matrix! \nThe matrix has been written to the maTran.txt\n\n" << endl;

    // Đóng file
    outputFile.close();

    // Giải phóng bộ nhớ của mảng hai chiều
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
    return 0;    
}

void docFile() 
{
    ifstream inputFile("maTran.txt");

    if (!inputFile.is_open()) 
	{
        cerr << "Không thể mở tệp tin." << endl;
        return;
    }

    inputFile >> n >> m;

    for (int i = 0; i < n; i++) 
	{
        for (int j = 0; j < m; j++) 
		{
            inputFile >> A[i][j];
        }
    }

    inputFile.close();
}

void resetMang() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            A[i][j] = originalA[i][j];
        }
    }
}

void giaoDienHuongDan()
{
	const char* text = "Beginner's Guide";
	const char* loiHuongDan = "Welcome to maze games!\n\n\n"
	"Your task is to go from the starting \n\npoint to the finish point.\n\n\n"
	"Use arrow keys to move \n\nand avoid colliding with walls.\n\n\n"
	"Good luck!";
	Font font = LoadFont("taiNguyen/font/LuckiestGuy-Regular.ttf");
	DrawTextEx(font, text, (Vector2){270, 60}, 36, 2, WHITE);
	DrawTextEx(font, loiHuongDan, (Vector2){50, 150}, 30, 2, WHITE);
}

void hienThiThongBao(bool kiemTra)
{
    const char* thongBao;

    if (kiemTra)
    {	
		timDuongBFSTruyVet(A, n, m);
        thongBao = "HAVE PATH";
        
    }
    else
    {
        thongBao = "NO PATH"; 
    }
    DrawText(thongBao, 270, 400, 40, WHITE);
}


void giaoDienDoThi(int A[][MAX], int cot, int hang)
{
    Rectangle ranDom = {50, 830, 110, 40};
    Rectangle DFS = {180, 830, 110, 40};
    Rectangle BFS = {310, 830, 110, 40};
    Rectangle TEST = {440, 830, 110, 40};
    Rectangle dijkstra = {570, 830, 150, 40};
    Rectangle reMaTrix = {800, 70, 100, 50};
    
    DrawRectangleRec(ranDom, GREEN);
    DrawText("RANDOM", ranDom.x + 15, ranDom.y + 15, 20, WHITE);
	
	DrawRectangleRec(DFS, GREEN);
    DrawText("DFS", DFS.x + 30, DFS.y + 15, 20, WHITE);
    
    DrawRectangleRec(BFS, GREEN);
    DrawText("BFS", BFS.x + 30, BFS.y + 15, 20, WHITE);
    
    DrawRectangleRec(TEST, GREEN);
    DrawText("TEST", TEST.x + 23, TEST.y + 15, 20, WHITE);
    
    DrawRectangleRec(dijkstra, GREEN);
    DrawText("DIJKSTRA", dijkstra.x + 23, dijkstra.y + 15, 20, WHITE);
    
    DrawRectangleRec(reMaTrix, RED);
    DrawText("RETURN", reMaTrix.x + 10, reMaTrix.y + 15, 20, WHITE);
	
    if (CheckCollisionPointRec(GetMousePosition(), ranDom) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
	    taoMaTran();
		docFile();
	}
    
    if (CheckCollisionPointRec(GetMousePosition(), DFS) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
        timDuongDFSTruyVet(A, cot, hang, 0, 0);
    }
	   
    if (CheckCollisionPointRec(GetMousePosition(), BFS) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
	    	// Gọi hàm kiểm tra đường đi
	    if (kiemTraDuongDi(A, cot, hang)) 
	    {
	        // Nếu có đường đi, thực hiện BFS để tô màu đường đi
	        timDuongBFS(A, cot, hang);
	    } 
    }
     
    if (CheckCollisionPointRec(GetMousePosition(), dijkstra) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
    	if (kiemTraDuongDi(A, cot, hang)) 
    	{
        	distra(A, m, n);
        	
		}
    }
 
    if (CheckCollisionPointRec(GetMousePosition(), TEST) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
    	bool kiemTra = kiemTraDuongDi(A,n,m);
    	hienThiThongBao(kiemTra);
       	
    }
    
    if (CheckCollisionPointRec(GetMousePosition(), reMaTrix) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
    {
        resetMang() ;
    }
	for (int i = 0; i < hang; i++)
    {
        for (int j = 0; j < cot; j++)
        {
            if (A[i][j] == 2) // 2 là màu đỏ để đánh dấu đường đi
            {
                int X = 50 + j * 35;
                int Y = 70 + i * 35;
                DrawRectangle(X, Y, 35, 35, RED);
            }
        }
    }  
}

void ve(int A[][MAX], int hang, int cot) 
{
	Texture2D bucTuong = LoadTexture("taiNguyen/image/da35.png");
	Texture2D gameName = LoadTexture("taiNguyen/image/gameName5.png");
	Texture2D hinhNen = LoadTexture("taiNguyen/image/hang800.png");
	DrawTexture(hinhNen, 0, 00, WHITE);
	DrawTexture(gameName, 250, 20, WHITE);
	Color darkBlue = {12, 36, 97, 255};
  	for (int i = 0; i < hang; i++) 
    {
        for (int j = 0; j < cot; j++) 
        {
            int X = 50 + j * 35;
            int Y = 70 + i * 35;
            if(A[i][j] == 0	)
            	DrawTexture(bucTuong, X, Y, WHITE);
            else if (A[i][j] == 2) 
                DrawRectangle(X, Y, 35, 35, RED); // Màu đỏ cho đường đi
        	else if( A[i][j] == 3) 
            	DrawRectangle(X, Y, 35, 35 , GREEN);   
            else if( A[i][j] == 9999) 
            	DrawRectangle(X, Y, 35, 35 , YELLOW);   
            else 
            	DrawRectangle(X, Y, 35, 35 , darkBlue);   
        }
    }
}

void timDuongBFS(int A[][MAX], int cot, int hang)
{
    vector<vector<bool>> visited(hang, vector<bool>(cot, false));
    queue<pair<int, int>> q; // Sử dụng pair để lưu tọa độ của ô

    // Bắt đầu từ ô (0, 0)
    q.push({0, 0});
    visited[0][0] = true;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (!q.empty())
    {
        pair<int, int> curr = q.front();
        q.pop();

        int x = curr.first;
        int y = curr.second;

        // Đổi màu ô thành đỏ
        A[x][y] = 2;

        // Kiểm tra nếu đã đến vị trí đích (hang - 1, cot - 1)
        if (x == hang - 1 && y == cot - 1)
        {
            break; // Đã tìm thấy đường đi, thoát khỏi vòng lặp
        }

        for (int i = 0; i < 4; i++)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (newX >= 0 && newX < hang && newY >= 0 && newY < cot && !visited[newX][newY] && A[newX][newY] == 1 )
            {
                q.push({newX, newY});
                visited[newX][newY] = true;
                ve(A, cot, hang);
            }
        }
    }
}

bool kiemTraDuongDi(int A[][MAX], int cot, int hang) 
{
    vector<vector<bool>> visited(hang, vector<bool>(cot, false));
    queue<pair<int, int>> q;

    q.push({0, 0});
    visited[0][0] = true;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (!q.empty()) 
    {
        pair<int, int> curr = q.front();
        q.pop();

        int x = curr.first;
        int y = curr.second;

        if (x == hang - 1 && y == cot - 1) 
        {
            return true; // Tìm thấy đường đi
        }

        for (int i = 0; i < 4; i++) 
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (newX >= 0 && newX < hang && newY >= 0 && newY < cot && !visited[newX][newY] && ( A[newX][newY] == 1 or A[newX][newY] == 2 or A[newX][newY] == 3)) 
            {
                q.push({newX, newY});
                visited[newX][newY] = true;
            }
        }
    }

    return false; // Không tìm thấy đường đi
}

void timDuongBFSTruyVet(int A[][MAX], int cot, int hang)
{
    vector<vector<bool>> visited(hang, vector<bool>(cot, false));
    vector<vector<int>> truyVet(hang, vector<int>(cot, -1)); // Mảng truy vết

    queue<pair<int, int>> q; // Sử dụng pair để lưu tọa độ của ô

    // Bắt đầu từ ô (0, 0)
    q.push({0, 0});
    visited[0][0] = true;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (!q.empty())
    {
        pair<int, int> curr = q.front();
        q.pop();

        int x = curr.first;
        int y = curr.second;

        // Kiểm tra nếu đã đến vị trí đích (hang - 1, cot - 1)
        if (x == hang - 1 && y == cot - 1)
        {
            // Xây dựng đường đi từ đích đến điểm đầu bằng cách theo dõi trạng thái truy vết
            vector<pair<int, int>> duongDi;
            pair<int, int> current = {hang - 1, cot - 1};
            while (current != make_pair(0, 0))
            {
                duongDi.push_back(current);
                current = {truyVet[current.first][current.second] / MAX, truyVet[current.first][current.second] % MAX};
            }
            duongDi.push_back({0, 0});

            // Đánh dấu đường đi trên ma trận A
            for (std::vector<std::pair<int, int>>::size_type i = 0; i < duongDi.size(); i++)
            {
                A[duongDi[i].first][duongDi[i].second] = 3;
            }

            break; // Đã tìm thấy đường đi, thoát khỏi vòng lặp
        }

        for (int i = 0; i < 4; i++)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (newX >= 0 && newX < hang && newY >= 0 && newY < cot && !visited[newX][newY] && A[newX][newY] == 1)
            {
                q.push({newX, newY});
                visited[newX][newY] = true;
                truyVet[newX][newY] = x * MAX + y; // Lưu trạng thái truy vết với giá trị 3
            }
        }
    }
}


void timDuongDFSTruyVet(int A[][MAX], int cot, int hang, int x, int y)
{
    // Kiểm tra nếu đang ở điểm đích (hang - 1, cot - 1)
    if (x == hang - 1 && y == cot - 1)
    {
        A[x][y] = 3; // Đánh dấu ô cuối là 3
        return;
    }

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++)
    {
        int newX = x + dx[i];
        int newY = y + dy[i];

        // Kiểm tra điều kiện hợp lệ để di chuyển
        if (newX >= 0 && newX < hang && newY >= 0 && newY < cot && A[newX][newY] == 1)
        {
            A[x][y] = 3; // Đánh dấu ô hiện tại là 3

            // Đệ quy để di chuyển đến ô tiếp theo
            timDuongDFSTruyVet(A, cot, hang, newX, newY);

            // Nếu đã tìm thấy đường đi, thoát khỏi vòng lặp
            if (A[hang - 1][cot - 1] == 3)
                return;
        }
    }
}

bool HopLe(int x, int y) 
{
    return (x >= 0 && x < n && y >= 0 && y < m && A[x][y] != 0);
}

// Hàm Dijkstra để tìm đường đi ngắn nhất và cập nhật mảng A
void distra(int A[MAX][MAX], int m, int n) 
{
    vector<vector<int>> khoangCach(n, vector<int>(m, INT_MAX));
    vector<vector<bool>> daTham(n, vector<bool>(m, false));
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>>
        hangDoi;

    int xBatDau = 0, yBatDau = 0;
    int xKetThuc = n - 1, yKetThuc = m - 1;

    khoangCach[xBatDau][yBatDau] = A[xBatDau][yBatDau];
    hangDoi.push({khoangCach[xBatDau][yBatDau], {xBatDau, yBatDau}});

    while (!hangDoi.empty()) {
        int xHienTai = hangDoi.top().second.first;
        int yHienTai = hangDoi.top().second.second;
        hangDoi.pop();

        if (xHienTai == xKetThuc && yHienTai == yKetThuc) {
            break;
        }

        if (daTham[xHienTai][yHienTai]) {
            continue;
        }

        daTham[xHienTai][yHienTai] = true;

        // Các ô lân cận
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};

        for (int i = 0; i < 4; i++) {
            int xMoi = xHienTai + dx[i];
            int yMoi = yHienTai + dy[i];

            if (HopLe(xMoi, yMoi)) {
                int duongDiMoi = khoangCach[xHienTai][yHienTai] + A[xMoi][yMoi];

                if (duongDiMoi < khoangCach[xMoi][yMoi]) {
                    khoangCach[xMoi][yMoi] = duongDiMoi;
                    hangDoi.push({khoangCach[xMoi][yMoi], {xMoi, yMoi}});
                }
            }
        }
    }

    // Lấy đường đi ngắn nhất từ (0, 0) đến (n-1, m-1)
    int x = xKetThuc;
    int y = yKetThuc;
    while (x != xBatDau || y != yBatDau) {
        int duongDiHienTai = khoangCach[x][y];
        A[x][y] = 9999; // Cập nhật giá trị của ô trên đường đi
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};

        for (int i = 0; i < 4; i++) {
            int xMoi = x + dx[i];
            int yMoi = y + dy[i];

            if (HopLe(xMoi, yMoi) && khoangCach[xMoi][yMoi] < duongDiHienTai) {
                x = xMoi;
                y = yMoi;
                break;
            }
        }
    }
    A[0][0] = 9999;
}