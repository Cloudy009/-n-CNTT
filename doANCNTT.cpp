#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <random>
#include <cstdlib>
#include <set> 
#include <limits>
#define MAX 100
using namespace std;
int A[MAX][MAX];
int n,m;
// Định nghĩa kiểu dữ liệu T cho thông tin của mỗi đỉnh
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

void XuatMenu() 
{
    cout << "Menu:" << endl;
    cout << "1. Xuat do thi " << endl;
    cout << "2. Thong tin dinh" << endl;
    cout << "3. Thong tin canh" << endl;
    cout << "4. Ma tran trong so" << endl;
    cout << "5. Ma tran ke" << endl;
    cout << "6. Ma tran lien ket" << endl;
    cout << "7. Kiem tra chu trinh Euler" << endl;
    cout << "8. Duyet BFS" << endl;
    cout << "9. Duyet DFS" << endl;
    cout << "10. Tao lai do thi" << endl;
    cout << "11. Tim duong di ngan nhat tu 1 dinh den cac dinh con lai" << endl;
   	cout << "12. Xuat mang A" << endl;
    cout << "13. Cay con nho nhat" << endl; 
	cout << "14. Xoa man hinh" << endl;
	cout << "15. Thoat!" << endl;
}

int main() {
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


    // Thêm cạnh giữa các đỉnh để tạo cấu trúc logic
    dothi.ThemCanh(0, 1, 5);
    dothi.ThemCanh(0, 0, 1);
    dothi.ThemCanh(9, 9, 3);
	dothi.ThemCanhNgauNhien(57,0,9);

    int choice;
    while (true) {
        XuatMenu();
        cout << "Chon chuc nang: ";
        cin >> choice;

        switch (choice) {
        	case 1:
        		dothi.XuatDoThi();
        		break;
            case 2:
                dothi.XuatThongTinDinh(2);
                break;
            case 3:
                dothi.XuatThongTinCanh(0, 2);
                break;
            case 4:
                // Xóa ma trận trọng số cũ trước khi tạo mới
//                dothi.XoaMaTranTrongSo();
                dothi.TaoMaTranTrongSo();
                cout << "MA TRAN TRONG SO:" << endl;
                dothi.XuatMaTranTrongSo();
                cout << endl;
                break;
            case 5:
                cout << "MA TRAN KE:" << endl;
                dothi.XuatMaTranKe();
                cout << endl;
                break;
            case 6:
                cout << "MA TRAN LIEN KET:" << endl;
                dothi.XuatMaTranLienKet();
                cout << endl;
                break;
            case 7:
                if (dothi.CoChuTrinhEuler()) {
                    cout << "Do thi co chu trinh EULER." << endl;
                } else {
                    cout << "Do thi KHONG CO chu trinh EULER." << endl;
                }
                break;
            case 8:
                cout << "Duyet do thi theo chieu RONG (BFS):" << endl;
                dothi.DuyetBFS(0);
                cout << endl;
                break;
            case 9:
                cout << "Duyet do thi theo chieu SAU (DFS):" << endl;
                dothi.DuyetDFSChieuSau(0);
                cout << endl;
                break;
            case 10:
//                dothi.XoaMaTranTrongSo();
//                dothi.TaoMaTranTrongSo();
                dothi.DatLaiTrongSo(0,9);
                break;
            case 11:
            	// Tìm đường đi ngắn nhất từ đỉnh 0 (A) đến tất cả các đỉnh còn lại
    			dothi.TimDuongNganNhatDijkstra(0);
    			break;
            case 12:
            	cout << "So hang (n): " << n << endl;
    			cout << "So cot (m): " << m << endl;
            	for( int i=0; i<n; i++)
            	{
					for(int j=0; j < m; j++)
            		{
						cout<<A[i][j]<<" ";
					}
					cout<<endl;
				}
				break;
			case 13:
				dothi.Prim();
				break;
			case 14:
                XoaManHinh();
                cin.ignore();
                cout << "Nhan Enter de tiep tuc...";
                cin.get();
                break;
            case 15:
                return 0;
            case 16:
            	chuyenMaTranAsangke();
            	break;
            default:
                cout << "Chức năng không hợp lệ. Vui lòng chọn lại." << endl;
                break;
        }
    }
    return 0;
}



