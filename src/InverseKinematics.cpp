#include "InverseKinematics.h"

Ankle_Parallel_Mechanism::Ankle_Parallel_Mechanism(){
    N1 = 19;
    N2 = 56;
    N3 = 37;
    N4 = 11;
    L1 = 73;
    L2 = 85;
    L3 = 50;
    L4 = 37;
    N5 = 10;
}

Ankle_Parallel_Mechanism::~Ankle_Parallel_Mechanism(){

}

void Ankle_Parallel_Mechanism::get_angle(double ankle_q1, double ankle_q2, double ankle_ql, double ankle_qr){
    Vector3d C_L;
    C_L << -N3*cos(ankle_q2)+(N4-N1)*sin(ankle_q2), 
        -N3*sin(ankle_q1)*sin(ankle_q2)-N2*cos(ankle_q1)-(N4-N1)*sin(ankle_q1)*cos(ankle_q2)+N5*sin(ankle_q1),
        N3*cos(ankle_q1)*sin(ankle_q2)-N2*sin(ankle_q1)+(N4-N1)*cos(ankle_q1)*cos(ankle_q2)-N5*cos(ankle_q1)+N1+N5;
    Vector3d C_R;
    C_R << N3*cos(ankle_q2)+(N4-N1)*sin(ankle_q2), 
        N3*sin(ankle_q1)*sin(ankle_q2)-N2*cos(ankle_q1)-(N4-N1)*sin(ankle_q1)*cos(ankle_q2)+N5*sin(ankle_q1),
        -N3*cos(ankle_q1)*sin(ankle_q2)-N2*sin(ankle_q1)+(N4-N1)*cos(ankle_q1)*cos(ankle_q2)-N5*cos(ankle_q1)+N1+N5;
    
    //left
    Vector3d A_L;
    A_L << -L4, 0, N1+N5+L1;
    Vector3d rc_ra_L = C_L-A_L;
    double lx = -rc_ra_L(1);
    double ly = rc_ra_L(2);
    double l1 = L3;
    double l2 = sqrt(pow(L2,2)-pow(rc_ra_L(0),2));
    double a = pow(lx,2)+pow(ly,2);
    double b = (-pow(lx,2)-pow(ly,2)-pow(l1,2)+pow(l2,2))*lx;
    double c = 0.25*pow((-pow(lx,2)-pow(ly,2)-pow(l1,2)+pow(l2,2)),2)-pow(l1,2)*pow(ly,2);
    double x = (-b+sqrt(pow(b,2)-4*a*c))/(2*a);
    double y = -lx/ly*x + (pow(lx,2)+pow(ly,2)+pow(l1,2)-pow(l2,2))/ly/2;
    theta_l = asin(y/l1);

    //right
    Vector3d A_R;
    A_R << L4,0,N1+N5+L1;
    Vector3d rc_ra_R = C_R-A_R;
    lx = -rc_ra_R(1);
    ly = rc_ra_R(2);
    l1 = L3;
    l2 = sqrt(pow(L2,2)-pow(rc_ra_R(0),2));
    a = pow(lx,2)+pow(ly,2);
    b = (-pow(lx,2)-pow(ly,2)-pow(l1,2)+pow(l2,2))*lx;
    c = 0.25*pow((-pow(lx,2)-pow(ly,2)-pow(l1,2)+pow(l2,2)),2)-pow(l1,2)*pow(ly,2);
    x = (-b+sqrt(pow(b,2)-4*a*c))/(2*a);
    y = -lx/ly*x + (pow(lx,2)+pow(ly,2)+pow(l1,2)-pow(l2,2))/ly/2;
    theta_r = asin(y/l1);
}

void Ankle_Parallel_Mechanism::get_file_data(){
    ifstream file_q;
    file_q.open("../data.txt", ios::in);
    if(!file_q.is_open()){
        cout << "文件打开失败" <<endl;
        return;
    }
    double tmp_data;
    int num = 0;
    vector<double> v1, v2, vl, vr;
    while(!file_q.eof()){
        file_q >> tmp_data;
        switch(num){
            case 0:
                v1.push_back(tmp_data);
                break;
            case 1:
                v2.push_back(tmp_data);
                break;
            case 2:
                vl.push_back(tmp_data);
                break;
            case 3:
                vr.push_back(tmp_data);
                break;
            default:
                break;
        }
        num++;
        num = (num == 4) ? 0 : num;
    }
    // cout << "v1:";
    // for(vector<double>::iterator it = v1.begin(); it != v1.end(); it++){
    //     cout << *it << "\t";
    // }
    // cout << endl;
    // cout << "--------------------------------------------";
    // cout << "\nv2:";
    // for(vector<double>::iterator it = v2.begin(); it != v2.end(); it++){
    //     cout << *it << "\t";
    // }
    // cout << endl;
    // cout << "--------------------------------------------";
    // cout << "\nvl:";
    // for(vector<double>::iterator it = vl.begin(); it != vl.end(); it++){
    //     cout << *it << "\t";
    // }
    // cout << endl;
    // cout << "--------------------------------------------";
    // cout << "\nvr:";
    // for(vector<double>::iterator it = vr.begin(); it != vr.end(); it++){
    //     cout << *it << "\t";
    // }
    vector<double> vl_calc, vr_calc;
    for(int i = 0; i < v1.size(); i++){
        get_angle(v1[i],v2[i],theta_l,theta_r);
        vl_calc.push_back(theta_l);
        vr_calc.push_back(theta_r);
    }
    vector<double> vl_diff, vr_diff;

    for(int i = 0; i < v1.size(); i++){
        vl_diff.push_back(vl[i] - vl_calc[i]);
        vr_diff.push_back(vr[i] - vr_calc[i]);
    }
    cout << "vl_diff:";
    for(vector<double>::iterator it = vl_diff.begin(); it != vl_diff.end(); it++){
        cout << *it << "\t";
    }
    cout << endl;
    cout << "--------------------------------------------";
    cout << "\nvr_diff:";
    for(vector<double>::iterator it = vr_diff.begin(); it != vr_diff.end(); it++){
        cout << *it << "\t";
    }
}


Poi_Move::Poi_Move(){

}

Poi_Move::~Poi_Move(){

}

double Poi_Move::poi_move(double poi_start, double poi_end, double speed, double time_step){
    if(poi_end - poi_start == 0){
        return 0;
    }
    static double target = poi_start;
    target += speed * time_step;
    target = target < poi_end ? target : poi_end;
    target = target > poi_start ? target : poi_start;
    if(target == poi_start){
        speed = fabs(poi_end - poi_start)/(poi_end - poi_start) * fabs(speed);
    }
    if(target == poi_end){
        speed = - fabs(poi_end - poi_start)/(poi_end - poi_start) * fabs(speed);
    }
    return target;
}