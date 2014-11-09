#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
const gchar * filename = "./FindYourGuys.ar";//路径或文件名
//主界面
static GtkWidget* control_window;
static GtkWidget *control_table;
//包含所有子界面控件的盒子
static GtkWidget *bigbox;
//姓名/学号/QQ/TEL
static GtkWidget *box1;
static GtkWidget *box2;
static GtkWidget *box3;
static GtkWidget *box4;
//提示信息
static GtkWidget *box5;
//textview
static GtkWidget *box6;
//确认按钮
static GtkWidget *box7;
//新建文件夹
static GtkWidget *box8;
//生日
static GtkWidget *box9;
//帮助界面的盒子
static GtkWidget *box10;
//姓名/学号/QQ/TEL的输入
static GtkWidget* entry1;
static GtkWidget* entry2;
static GtkWidget* entry3;
static GtkWidget* entry4;
//文件名的输入
static GtkWidget* entry5;
//生日的输入
static GtkWidget *m_spin = NULL; //月份
static GtkWidget *d_spin = NULL; //号数
//提示信息的标签
static GtkWidget* label5;
//菜单栏
static GtkWidget *toolbar;
//textview组件
static GtkWidget *text;
GtkTextBuffer *buffer;
//确认按钮和分割线
static GtkWidget *comfirm_button;
static GtkWidget* sep;
//子界面切换--互斥的信号量
int INSERT=0;
int ALTER=0;
int DELETE=0;
int FIND=0;
int NEW=0;
int HELP=0;
//帮助子界面的几个frame和label
GtkWidget *frame1,*frame2,*frame3,*frame4;
GtkWidget *labelx,*labely,*labelz;
//获取当前系统时间
time_t ti;
struct tm* timeinfo;
//一个联系方式的数据结构体
struct contact
{
	char name[30];
	char number[20];
	char qq[12];
	char tel[15];
	gdouble month;
	gdouble day;
};
struct contact coninfo;//单个读取
struct contact infoset[300];//查找操作结果集
struct contact all_coninfo[1000];//本文件里面所有的联系方式
int load_data()
{
	int i=0;
	FILE *fp;
	if((fp = fopen(filename,"r")) == NULL)
	{
		return;
	}
	memset(&coninfo,0x00,sizeof(coninfo));
	while(fread(&coninfo,sizeof(coninfo),1,fp) == 1)
	{
		strcpy(all_coninfo[i].name,coninfo.name);
		strcpy(all_coninfo[i].number,coninfo.number);
		strcpy(all_coninfo[i].qq,coninfo.qq);
		strcpy(all_coninfo[i].tel,coninfo.tel);
		all_coninfo[i].month=coninfo.month;
		all_coninfo[i].day=coninfo.day;
		i++;
		memset(&coninfo,0x00,sizeof(coninfo));
	}
	fclose(fp);
	return i;
}
//确认按钮的监听函数
void on_comfirm_button_clicked()
{
	gdouble month = gtk_spin_button_get_value(GTK_SPIN_BUTTON(m_spin));
	gdouble day = gtk_spin_button_get_value(GTK_SPIN_BUTTON(d_spin));
 	const gchar *name = gtk_entry_get_text(GTK_ENTRY(entry1));
	const gchar *number = gtk_entry_get_text(GTK_ENTRY(entry2));
	const gchar *qq = gtk_entry_get_text(GTK_ENTRY(entry3));
	const gchar *tel = gtk_entry_get_text(GTK_ENTRY(entry4));
	FILE *fp;
	int found;
	gchar query_buf[1024];
	GtkTextIter iter;
	GtkTextIter start,end;
	FILE *fpn;
	int i=0,h;
	if(NEW)
	{
		filename = gtk_entry_get_text(GTK_ENTRY(entry5));
	}
	if(INSERT)
	{
		//g_print("INSERt....");
		if((fp = fopen(filename,"a")) == NULL)
		{
			return;
		}
		memset(&coninfo,0x00,sizeof(coninfo));
		strncpy(coninfo.name,name,30);
		strncpy(coninfo.number,number,20);
		strncpy(coninfo.qq,qq,12);
		strncpy(coninfo.tel,tel,15);
		coninfo.month = month;
		coninfo.day = day;
		if(fwrite(&coninfo,sizeof(coninfo),1,fp) < 0)
		{
			perror("fwrite");
			fclose(fp);
			return;
		}
		fclose(fp);
		gtk_label_set_text(GTK_LABEL(label5),"添加成功");
		int j = load_data();
		gtk_text_buffer_get_start_iter(buffer,&start);
		gtk_text_buffer_get_end_iter(buffer,&end);
		gtk_text_buffer_delete(buffer,&start,&end);//清除所有
		for(h=1;h<j+1;h++)
		{
			if(strcmp(all_coninfo[h].name,"") > 0)
				sprintf(query_buf,"姓名：%s\t\t",all_coninfo[h].name);
			if(h % 5 == 0)
				strcat(query_buf,"\n");
			gtk_text_buffer_get_end_iter(buffer,&iter);
			gtk_text_buffer_insert(buffer,&iter,query_buf,-1);
		}
	}
	if(ALTER)
	{
		//g_print("ALTER......");
		found = 0;
		if((fpn = fopen("./tmpfile","w")) == NULL)
		{
			return;
		}
		if((fp = fopen(filename,"r")) == NULL)
		{
			return;
		}
		memset(&coninfo,0x00,sizeof(coninfo));//清空结构
    
		while(fread(&coninfo,sizeof(coninfo),1,fp) == 1)//循环复制，与输入姓名相匹配的不复制
		{
			if(strcmp(name,coninfo.name) == 0)//相同则修改
			{
				found =1;
				if(strcmp(coninfo.number,"") != 0)
					strcpy(coninfo.number,number);
				if(strcmp(coninfo.qq,"") != 0)
					strcpy(coninfo.qq,qq);
				if(strcmp(coninfo.tel,"") != 0)
					strcpy(coninfo.tel,tel);
				if(coninfo.month != month)
					coninfo.month = month;
				if(coninfo.day != day)
					coninfo.day = day;
			}
			fwrite(&coninfo,sizeof(coninfo),1,fpn);
			memset(&coninfo,0x00,sizeof(coninfo));
		}
		fclose(fp);
		fclose(fpn);
		remove(filename);//删除原档案文件
		rename("./tmpfile",filename);//复制好的新文件重命名为档案文件
		if(found)
			gtk_label_set_text(GTK_LABEL(label5),"修改成功");
		else
			gtk_label_set_text(GTK_LABEL(label5),"没有找到该同学的联系方式");
	}
	if(DELETE)
	{
		//g_print("DELETE......");
		if((fpn = fopen("./tmpfile","w")) == NULL)
		{
			return;
		}
		if((fp = fopen(filename,"r")) == NULL)
		{
			return;
		}
		memset(&coninfo,0x00,sizeof(coninfo));//清空结构
		while(fread(&coninfo,sizeof(coninfo),1,fp) == 1)//循环复制，与输入姓名相匹配的不复制
		{
			if(strcmp(name,coninfo.name) != 0)
			{
				fwrite(&coninfo,sizeof(coninfo),1,fpn);//不相同，则复制
			}
			memset(&coninfo,0x00,sizeof(coninfo));
		}
		fclose(fp);
		fclose(fpn);
		remove(filename);//删除原档案文件
		rename("./tmpfile",filename);//复制好的新文件重命名为档案文件
		gtk_label_set_text(GTK_LABEL(label5),"删除成功");
		int j = load_data();
		gtk_text_buffer_get_start_iter(buffer,&start);
		gtk_text_buffer_get_end_iter(buffer,&end);
		gtk_text_buffer_delete(buffer,&start,&end);//清除所有
		for(h=1;h<j+1;h++)
		{
			if(strcmp(all_coninfo[h].name,"") > 0)
				sprintf(query_buf,"姓名：%s\t\t",all_coninfo[h].name);
			if(h % 5 == 0)
				strcat(query_buf,"\n");
			gtk_text_buffer_get_end_iter(buffer,&iter);
			gtk_text_buffer_insert(buffer,&iter,query_buf,-1);
		}
	}
	if(FIND)
	{
		//g_print("FIND........");
		if((fp = fopen(filename,"r")) == NULL)
		{
			return;
		}
		memset(&coninfo,0x00,sizeof(coninfo));
		found=0;
		while(fread(&coninfo,sizeof(coninfo),1,fp) == 1)
		{
			if(strcmp(name,coninfo.name) == 0)
			{
				found=1;
				strcpy(infoset[i].name,coninfo.name);
				strcpy(infoset[i].number,coninfo.number);
				strcpy(infoset[i].qq,coninfo.qq);
				strcpy(infoset[i].tel,coninfo.tel);
				infoset[i].month=coninfo.month;
				infoset[i].day=coninfo.day;
				i++;
			}
			memset(&coninfo,0x00,sizeof(coninfo));
		}
		fclose(fp);
		gtk_text_buffer_get_start_iter(buffer,&start);
		gtk_text_buffer_get_end_iter(buffer,&end);
		gtk_text_buffer_delete(buffer,&start,&end);//清除所有
		while(found && i--)
		{
			sprintf(query_buf,"姓名：%s\t学号：%s\tQQ: %s\tTel: %s\t生日： %d月%d日 \n",
					infoset[i].name,infoset[i].number,infoset[i].qq,infoset[i].tel,(int)infoset[i].month,(int)infoset[i].day);
			gtk_text_buffer_get_end_iter(buffer,&iter);
			gtk_text_buffer_insert(buffer,&iter,query_buf,-1);
		}
		if(!found)
		{
			gtk_text_buffer_get_end_iter(buffer,&iter);
			gtk_text_buffer_insert(buffer,&iter,
			"没有该名同学的资料",-1);
		}
 
		gtk_text_buffer_get_end_iter(buffer,&iter);
		gtk_text_buffer_insert(buffer,&iter,"\n",-1);
	}
}
//子界面控件初始化
void Widget_init()
{
	GtkWidget* label1;
	GtkWidget* label2;
	GtkWidget* label3;
	GtkWidget* label4;
	GtkWidget* label6;
	//birthday
	GtkWidget* label7;
	GtkWidget* label8;
	GtkWidget* label9;
	//创建横向盒
	box5 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box5,FALSE,TRUE,5);
	box10 = gtk_vbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box10,FALSE,TRUE,5);
	box1 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box1,FALSE,TRUE,5);
	box2 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box2,FALSE,FALSE,5);
	box3 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box3,FALSE,FALSE,5);
	box4 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box4,FALSE,FALSE,5);
	box8 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box8,FALSE,FALSE,5);
	box9 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box9,FALSE,FALSE,5);
	box7 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box7,FALSE,FALSE,5);
	//创建1个横向分隔线，并添加到纵向的盒子中
	sep = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(bigbox),sep,FALSE,FALSE,5);
	//创建一个包含textview的盒子
	box6 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box6,FALSE,FALSE,5);
	//创建一个“姓名”标签
	label1 = gtk_label_new("                                                 姓名:");
	//创建一个输入框
	entry1 = gtk_entry_new();
	//按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box1),label1,FALSE,FALSE,5);
	gtk_label_set_justify(GTK_LABEL(label1),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box1),entry1,FALSE,FALSE,5);
	//创建一个“学号”标签
	label2 = gtk_label_new("                                                 学号:");
	//创建一个输入框
	entry2 = gtk_entry_new();
	//按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box2),label2,FALSE,FALSE,5);
	gtk_label_set_justify(GTK_LABEL(label2),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box2),entry2,FALSE,FALSE,5);
	//创建一个“QQ”标签
	label3 = gtk_label_new("                                                 QQ   :");
	//创建一个输入框
	entry3 = gtk_entry_new();
	//按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box3),label3,FALSE,FALSE,5);
	gtk_label_set_justify(GTK_LABEL(label3),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box3),entry3,FALSE,FALSE,5);
	//创建一个“TEL”标签
	label4 = gtk_label_new("                                                 TEL  :");
	//创建一个输入框
	entry4 = gtk_entry_new();
	//按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box4),label4,FALSE,FALSE,5);
	gtk_label_set_justify(GTK_LABEL(label4),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box4),entry4,FALSE,FALSE,5);
	//创建一个“TEL”标签
	label5 = gtk_label_new("欢迎使用收集器");
	gtk_label_set_justify(GTK_LABEL(label5),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box5),label5,TRUE,TRUE,5);
	//创建一个“新建文件”标签
	label6 = gtk_label_new("                                   新建文件名:");
	//创建一个输入框
	entry5 = gtk_entry_new();
	//按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box8),label6,FALSE,FALSE,5);
	gtk_label_set_justify(GTK_LABEL(label2),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box8),entry5,FALSE,FALSE,5);
	//创建一个textview
	text = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(box6),text,TRUE,TRUE,5);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
	//创建一个“生日”的标签,一个月的标签，一个日的标签
	label7 = gtk_label_new("                                                 生日:");
	label8 = gtk_label_new("月");
	label9 = gtk_label_new("日");
	//创建两个月和日的输入
	m_spin = gtk_spin_button_new_with_range(1,12,1);
	d_spin = gtk_spin_button_new_with_range(1,31,1);
	////按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box9),label7,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box9),m_spin,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box9),label8,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box9),d_spin,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box9),label9,FALSE,FALSE,5);
	//创建多个frame来写提示信息
	frame1 = gtk_frame_new("操作说明");
	labelx = gtk_label_new("1.第一次使用入录器，需要先按【新建】按钮，新建一个存储文件\n2.如果已有存储文件，按【打开】按钮，选择打开相应文件\n3.[注意]如果没有确保有存储文件，默认存储文件为FindYourGuys.ar\n");
	gtk_container_add(GTK_CONTAINER(frame1),labelx);
	gtk_label_set_justify(GTK_LABEL(labelx),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box10),frame1,FALSE,TRUE,5);
	frame2 = gtk_frame_new("联系方式的规格说明");
	labely = gtk_label_new("本录入器很遗憾没有添加检查错误的功能\n只能希望大家如实填写，姓名重名的删除时是一律删除的\n姓名最好通过标记来分辨性别和同名人\n如【张三|男|1】【张三|男|2】[张三|女】");
	gtk_container_add(GTK_CONTAINER(frame2),labely);
	gtk_label_set_justify(GTK_LABEL(labely),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box10),frame2,FALSE,TRUE,5);
	frame3 = gtk_frame_new("作者 && 源码");
	labelz = gtk_label_new("本作品开源，代码比较冗余，有待改进\ngithub，搜索FindYourGuys，即能搜索到\n作者：苏超然（Richard Sucran）");
	gtk_container_add(GTK_CONTAINER(frame3),labelz);
	gtk_label_set_justify(GTK_LABEL(labelz),GTK_JUSTIFY_CENTER);
	gtk_box_pack_start(GTK_BOX(box10),frame3,FALSE,TRUE,5);
	//创建确认按钮
	comfirm_button = gtk_button_new_with_label("确认");
	g_signal_connect(G_OBJECT(comfirm_button),"clicked",G_CALLBACK(on_comfirm_button_clicked),NULL);
	gtk_box_pack_start(GTK_BOX(box7),comfirm_button,TRUE,TRUE,5);
}
//子界面控件隐藏
void Widget_hide()
{
	gtk_widget_hide_all(box1);
	gtk_widget_hide_all(box2);
	gtk_widget_hide_all(box3);
	gtk_widget_hide_all(box4);
	gtk_widget_hide_all(box5);
	gtk_widget_hide_all(box6);
	gtk_widget_hide_all(box8);
	gtk_widget_hide_all(box9);
	gtk_widget_hide_all(box10);
}
//文件选择函数
void on_file_select_ok(GtkWidget * button,GtkFileSelection *fs)
{
    GtkWidget * dialog;	//对话框
    gchar message[1024];	//路径缓存
    
    //从参数 fs 中获取路径
    filename = gtk_file_selection_get_filename(fs);
    
    //判断是否为目录
    if(g_file_test(filename,G_FILE_TEST_IS_DIR))		
		sprintf(message,"你选择的目录是:%s",filename);	//字符串连接
    else
		sprintf(message,"你选择的文件是:%s",filename);
    dialog = gtk_message_dialog_new(NULL,
	    GTK_DIALOG_DESTROY_WITH_PARENT,
	    GTK_MESSAGE_INFO,
	    GTK_BUTTONS_OK,
	    message,
	    NULL);
    gtk_widget_destroy(GTK_WIDGET(fs));	//销毁文件选则对话框 fs
    gtk_dialog_run(GTK_DIALOG(dialog));	//显示dialog对话框并等待按钮,在有按钮按下之后继续
    gtk_widget_destroy(dialog);			//销毁dialog对话框
}
//菜单栏图标监听函数
void new_file_button_clicked()
{
	gtk_label_set_text(GTK_LABEL(label5),"请输入正确的路径，例如./2012级计算机实验班.ar");
	while(!NEW)
	{
		Widget_hide();
		gtk_widget_show_all(box5);
		gtk_widget_show_all(box8);
		gtk_widget_show_all(box7);
		INSERT=0;DELETE=0;ALTER=0;FIND=0;NEW=1;HELP=0;
	}
}
void open_file_button_clicked()
{
	GtkWidget * dialog;
    
    //创建文件选择对话框
    dialog = gtk_file_selection_new("请选择一个文件：");
    gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
    gtk_signal_connect(GTK_OBJECT(dialog),"destroy",
	    GTK_SIGNAL_FUNC(gtk_widget_destroy),&dialog);
	
	//给确定按钮添加回掉函数
    g_signal_connect(
			GTK_OBJECT(GTK_FILE_SELECTION(dialog)->ok_button),
			 "clicked",
		    GTK_SIGNAL_FUNC(on_file_select_ok),
		    GTK_FILE_SELECTION(dialog));

	//给取消按钮添加回掉函数
	gtk_signal_connect_object(
			GTK_OBJECT(GTK_FILE_SELECTION(dialog)->cancel_button),
			"clicked",
			GTK_SIGNAL_FUNC(gtk_widget_destroy),
			GTK_OBJECT(dialog));

	gtk_widget_show(dialog);
}
void on_insert_button_clicked()
{
	gtk_label_set_text(GTK_LABEL(label5),"请输入新同学的联系方式");
	while(!INSERT)
	{
		Widget_hide();
		gtk_widget_show_all(box1);
		gtk_widget_show_all(box2);
		gtk_widget_show_all(box3);
		gtk_widget_show_all(box4);
		gtk_widget_show_all(box5);
		gtk_widget_show_all(sep);
		gtk_widget_show_all(box7);
		gtk_widget_show_all(box9);
		gtk_widget_show_all(box6);
		INSERT=1;DELETE=0;ALTER=0;FIND=0;NEW=0;HELP=0;
	}
}
void on_alter_button_clicked()
{
	gtk_label_set_text(GTK_LABEL(label5),"按姓名输入要修改的信息");
	while(!ALTER)
	{
		Widget_hide();
		gtk_widget_show_all(box1);
		gtk_widget_show_all(box2);
		gtk_widget_show_all(box3);
		gtk_widget_show_all(box4);
		gtk_widget_show_all(box5);
		gtk_widget_show_all(box7);
		gtk_widget_show_all(box9);
		NEW=0;ALTER=1;INSERT=0;DELETE=0;FIND=0;HELP=0;
	}
}
void on_delete_button_clicked()
{
	gtk_label_set_text(GTK_LABEL(label5),"请输入要删除的同学姓名");
	while(!DELETE)
	{
		Widget_hide();
		gtk_widget_show_all(box1);
		gtk_widget_show_all(box5);
		gtk_widget_show_all(box7);
		gtk_widget_show_all(box6);
		DELETE=1;INSERT=0;ALTER=0;FIND=0;NEW=0;HELP=0;
	}
}
void on_find_button_clicked()
{
	gtk_label_set_text(GTK_LABEL(label5),"请输入要查找的同学姓名");
	while(!FIND)
	{
		Widget_hide();
		gtk_widget_show_all(box1);
		gtk_widget_show_all(box5);
		gtk_widget_show_all(box7);
		gtk_widget_show_all(box6);
		FIND=1;DELETE=0;INSERT=0;ALTER=0;NEW=0;HELP=0;
	}
}
void on_birthday_button_clicked()
{
	int found =0;
	GtkWidget * dialog;	//对话框
	gchar message[1024];
	gchar buffer[1024]="这个月：";
	int i = load_data();
	srand((unsigned)time(&ti));
	timeinfo = localtime(&ti);
	//g_print("zhege :%d\n",timeinfo->tm_mon +1);
	while(i--)
	{
		if(all_coninfo[i].month == timeinfo->tm_mon +1)
		{
			found=1;
			strcat(buffer,strcat(all_coninfo[i].name,"   "));
		}
	}
	if(!found)
		strcpy(message,"没有找到");
	else
		strcpy(message,buffer);
		strcat(message,"生日！");
	dialog = gtk_message_dialog_new(NULL,
	    GTK_DIALOG_DESTROY_WITH_PARENT,
	    GTK_MESSAGE_INFO,
	    GTK_BUTTONS_OK,
	    message,
	    NULL);
	gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));	//显示dialog对话框并等待按钮,在有按钮按下之后继续
    gtk_widget_destroy(dialog);			//销毁dialog对话框
}
void on_help_button_clicked()
{
	gtk_label_set_text(GTK_LABEL(label5),"请仔细查阅帮助信息");
	while(!HELP)
	{
		Widget_hide();
		gtk_widget_hide_all(box7);
		gtk_widget_show_all(box5);
		gtk_widget_show_all(box10);
		FIND=0;DELETE=0;INSERT=0;ALTER=0;NEW=0;HELP=1;
	}
}
//主界面实现
GtkWidget* create_control()
{
	//主操作界面设置
	control_table = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(control_table),"录入器主界面");
	g_signal_connect(G_OBJECT(control_table),"delete_event",G_CALLBACK(gtk_widget_destroy),control_table);
	gtk_window_set_position(GTK_WINDOW(control_table),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(control_table),10);
	gtk_window_set_default_size(GTK_WINDOW(control_table),550,550);
	
	//创建盒子
	bigbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(control_table),bigbox);

	//创建工具条
	toolbar = gtk_toolbar_new ();
	gtk_toolbar_set_orientation (GTK_TOOLBAR (toolbar), GTK_ORIENTATION_HORIZONTAL);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_BOTH);
	gtk_container_set_border_width (GTK_CONTAINER (toolbar), 5);
	gtk_box_pack_start(GTK_BOX(bigbox),toolbar,FALSE,FALSE,0);
	
	//创建带图片的按钮
	GtkWidget *insert_image;
	GtkWidget *button1;
	GtkWidget *delete_image;
	GtkWidget *button2;
	GtkWidget *alter_image;
	GtkWidget *button3;
	GtkWidget *find_image;
	GtkWidget *button4;
	GtkWidget *new_file_image;
	GtkWidget *button5;
	GtkWidget *open_alt_image;
	GtkWidget *button6;
	GtkWidget *birthday_image;
	GtkWidget *button7;
	GtkWidget *help_image;
	GtkWidget *button8;
	//因为gtk_toolbar_set_space_size，gtk2.0不支持，为了实现空格，用最笨的方法添加了空白的label
	GtkWidget *labela,*labelb,*labelc,*labeld,*labele,*labelf,*labelg,*labelh,*labeli,*labelj;
	new_file_image = gtk_image_new_from_file("./new_doc.png");
	button5 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"新建","新建一个通讯录","Private",new_file_image,G_CALLBACK(new_file_button_clicked),NULL);
	//增加间距
	labelc = gtk_label_new("   ");
	gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),labelc,"This is just an space","Private");
	gtk_widget_show(labelc);
	//
	open_alt_image = gtk_image_new_from_file("./open_alt.png");
	button6 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"打开","打开一个通讯录","Private",open_alt_image,G_CALLBACK(open_file_button_clicked),NULL);
	//增加间距
	labelb = gtk_label_new("      ");
	gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),labelb,"This is just an space","Private");
	gtk_widget_show(labelb);
	//
	gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));
	//增加间距
	labela = gtk_label_new("      ");
	gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),labela,"This is just an space","Private");
	gtk_widget_show(labela);
	//
	insert_image = gtk_image_new_from_file("./plus_2.png");
	button1 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"添加","添加一个学生的联系方式","Private",insert_image,G_CALLBACK(on_insert_button_clicked),NULL);
	//增加间距
	labeld = gtk_label_new("   ");
	gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),labeld,"This is just an space","Private");
	gtk_widget_show(labeld);
	//
	delete_image = gtk_image_new_from_file("./minus_2.png");
	button2 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"删除","删除一个学生的联系方式","Private",delete_image,G_CALLBACK(on_delete_button_clicked),NULL);
	//增加间距
	labele = gtk_label_new("   ");
	gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),labele,"This is just an space","Private");
	gtk_widget_show(labele);
	//
	alter_image = gtk_image_new_from_file("./pencil_edit.png");
	button3 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"修改","修改一个学生的联系方式","Private",alter_image,G_CALLBACK(on_alter_button_clicked),NULL);
	//增加间距
	labelf = gtk_label_new("   ");
	gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),labelf,"This is just an space","Private");
	gtk_widget_show(labelf);
	//
	find_image = gtk_image_new_from_file("./search_lense.png");
	button4 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"查询","查询一个学生的联系方式","Private",find_image,G_CALLBACK(on_find_button_clicked),NULL);
	//增加间距
	labelh = gtk_label_new("      ");
	gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),labelh,"This is just an space","Private");
	gtk_widget_show(labelh);
	//
	gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));
	//增加间距
	labeli = gtk_label_new("      ");
	gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),labeli,"This is just an space","Private");
	gtk_widget_show(labeli);
	//
	birthday_image = gtk_image_new_from_file("./cake.png");
	button7 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"生日","谁在这个月生日","Private",birthday_image,G_CALLBACK(on_birthday_button_clicked),NULL);
	//增加间距
	labelj = gtk_label_new("   ");
	gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),labelj,"This is just an space","Private");
	gtk_widget_show(labelj);
	//
	help_image = gtk_image_new_from_file("./help.png");
	button7 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"求助","遇到操作问题点这个","Private",help_image,G_CALLBACK(on_help_button_clicked),NULL);
	//设定工具条中图标大小，GTK_ICON_SIZE_SMALL_TOOLBAR表示工具条中使用小图标
	gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar),GTK_ICON_SIZE_SMALL_TOOLBAR);
	return control_table;
}
//主界面的显示
void  on_button_clicked  (GtkWidget* button,gpointer data)
{
	control_window = create_control();
	Widget_init();
	gtk_widget_show_all(box10);
	gtk_widget_show(toolbar);
	gtk_widget_show(bigbox);
	gtk_widget_show(control_window);
}
//登陆界面的实现
int main(int argc, char* argv[])
{
	//登陆窗体的设置和初始化
	GtkWidget* window;
	gtk_init(&argc, &argv);//初始化
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"登录窗口");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window),550,550);
	gtk_container_set_border_width(GTK_CONTAINER(window),20);
	//设置窗口布局
	GtkWidget* box;
	GtkWidget* box1;
	GtkWidget* image;
	GtkWidget* frame;
	GtkWidget* label1;
	box = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),box);
	box1 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),box1,FALSE,FALSE,5);
	//添加图片布局
	image = gtk_image_new_from_file("封面.png");
	gtk_container_add(GTK_CONTAINER(box1),image);
	//添加文字布局
	frame = gtk_frame_new("联系方式录入器");
	label1 = gtk_label_new("每一次告别，最好用力一点。\n多说一句，可能是最后一句。\n多看一眼，可能是最后一眼。\n ——韩寒\n");
	gtk_container_add(GTK_CONTAINER(frame),label1);
	gtk_label_set_justify(GTK_LABEL(label1),GTK_JUSTIFY_RIGHT);
	gtk_box_pack_start(GTK_BOX(box),frame,FALSE,FALSE,5);
	//添加确认按钮
	GtkWidget* button;
	button = gtk_button_new_with_label("开始录入，不忘联系");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_button_clicked),NULL);
	gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);
	gtk_widget_show_all(window);
	gtk_main();
	return FALSE;
}
