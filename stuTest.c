#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARFILE "./usr.ar"//指定档案文件的路径名称
static GtkWidget* entry1;
static GtkWidget* entry2;
static GtkWidget *blabel = NULL;//定义提示信息条
static GtkWidget* control_window;
static GtkWidget *control_table;
static GtkWidget *bigbox;
static GtkWidget *box1;
static GtkWidget *box2;
static GtkWidget *box3;
static GtkWidget *box4;
static GtkWidget *box5;
static GtkWidget* entry1;
static GtkWidget* entry2;
static GtkWidget* entry3;
static GtkWidget* entry4;
static GtkWidget* label5;
static GtkWidget *toolbar;
int INSERT=0;
int ALTER=0;
int DELETE=0;
int FIND=0;
void Widget_init()
{
	GtkWidget* label1;
	GtkWidget* label2;
	GtkWidget* label3;
	GtkWidget* label4;
	GtkWidget* sep;
	//创建4个横向盒
	box1 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box1,FALSE,FALSE,5);
	box2 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box2,FALSE,FALSE,5);
	box3 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box3,FALSE,FALSE,5);
	box4 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box4,FALSE,FALSE,5);
	box5 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(bigbox),box5,FALSE,FALSE,5);
	//创建1个横向分隔线，并添加到纵向的盒子中
	sep = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(bigbox),sep,FALSE,FALSE,5);
	//创建一个“姓名”标签
	label1 = gtk_label_new("姓名:");
	//创建一个输入框
	entry1 = gtk_entry_new();
	//按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box1),label1,FALSE,FALSE,5);
	gtk_label_set_justify(GTK_LABEL(label1),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box1),entry1,FALSE,FALSE,5);
	//创建一个“学号”标签
	label2 = gtk_label_new("学号:");
	//创建一个输入框
	entry2 = gtk_entry_new();
	//按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box2),label2,FALSE,FALSE,5);
	gtk_label_set_justify(GTK_LABEL(label2),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box2),entry2,FALSE,FALSE,5);
	//创建一个“QQ”标签
	label3 = gtk_label_new("QQ   :");
	//创建一个输入框
	entry3 = gtk_entry_new();
	//按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box3),label3,FALSE,FALSE,5);
	gtk_label_set_justify(GTK_LABEL(label3),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box3),entry3,FALSE,FALSE,5);
	//创建一个“TEL”标签
	label4 = gtk_label_new("TEL  :");
	//创建一个输入框
	entry4 = gtk_entry_new();
	//按从前到后顺序在一个横向盒子中排列这个标签和输入框
	gtk_box_pack_start(GTK_BOX(box4),label4,FALSE,FALSE,5);
	gtk_label_set_justify(GTK_LABEL(label4),GTK_JUSTIFY_LEFT);
	gtk_box_pack_start(GTK_BOX(box4),entry4,FALSE,FALSE,5);
	//创建一个“TEL”标签
	label5 = gtk_label_new("欢迎使用收集器");
	gtk_box_pack_start(GTK_BOX(box5),label5,FALSE,FALSE,5);
}
void Widget_hide()
{
	gtk_widget_hide_all(box1);
	gtk_widget_hide_all(box2);
	gtk_widget_hide_all(box3);
	gtk_widget_hide_all(box4);
	gtk_widget_hide_all(box5);
}
void on_insert_button_clicked()
{
	gtk_label_set_text(GTK_LABEL(label5),"请输入新同学的联系方式");
	while(!INSERT)
	{
		Widget_hide();
		gtk_widget_show_all(bigbox);
		INSERT=1;DELETE=0;ALTER=0;FIND=0;
	}
}
void on_alter_button_clicked()
{
	gtk_label_set_text(GTK_LABEL(label5),"按姓名输入要修改的信息");
	while(!ALTER)
	{
		Widget_hide();
		gtk_widget_show_all(bigbox);
		ALTER=1;INSERT=0;DELETE=0;FIND=0;
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
		DELETE=1;INSERT=0;ALTER=0;FIND=0;
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
		FIND=1;DELETE=0;INSERT=0;ALTER=0;
	}
}
GtkWidget* create_control()
{
	//主操作界面设置
	control_table = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(control_table),"录入器主界面");
	g_signal_connect(G_OBJECT(control_table),"delete_event",G_CALLBACK(gtk_widget_destroy),control_table);
	gtk_window_set_position(GTK_WINDOW(control_table),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(control_table),10);
	gtk_window_set_default_size(GTK_WINDOW(control_table),200,400);
	
	//创建盒子
	bigbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(control_table),bigbox);

	//创建工具条
	toolbar = gtk_toolbar_new( );
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
	insert_image = gtk_image_new_from_file("./plus_2.png");
	button1 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"添加","添加一个学生的联系方式","Private",insert_image,G_CALLBACK(on_insert_button_clicked),NULL);
	delete_image = gtk_image_new_from_file("./minus_2.png");
	button2 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"删除","删除一个学生的联系方式","Private",delete_image,G_CALLBACK(on_delete_button_clicked),NULL);
	alter_image = gtk_image_new_from_file("./pencil_edit.png");
	button3 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"修改","修改一个学生的联系方式","Private",alter_image,G_CALLBACK(on_alter_button_clicked),NULL);
	find_image = gtk_image_new_from_file("./search_lense.png");
	button4 = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),"查询","查询一个学生的联系方式","Private",find_image,G_CALLBACK(on_find_button_clicked),NULL);
	
	//设定工具条的外观,GTK_TOOLBAR_ICONS表示只显示图标
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_ICONS);
	//设定工具条中图标大小，GTK_ICON_SIZE_SMALL_TOOLBAR表示工具条中使用小图标
	gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar),GTK_ICON_SIZE_SMALL_TOOLBAR);
	return control_table;
}
void show_choose ()
{
	control_window = create_control();
	Widget_init();
	gtk_widget_show_all(control_window);
}
//用户名和密码验证
void  on_button_clicked  (GtkWidget* button,gpointer data)
{
	const gchar *username = gtk_entry_get_text(GTK_ENTRY(entry1));
	const gchar *password = gtk_entry_get_text(GTK_ENTRY(entry2));
	g_print("用户名是：%s ",username);
	g_print("\n");
	g_print("密码是：%s ",password);
	g_print("\n");
	const gchar *un="xjl";
	const gchar *pw="123";
	if(strcmp(username,un) == 0 && strcmp(password,pw) == 0)
	{
		g_print("right  \n");
		gtk_label_set_text(GTK_LABEL(blabel),"验证成功");
		show_choose () ;
	}
	else
	{
		g_print("again  \n");
		gtk_label_set_text(GTK_LABEL(blabel),"用户名或密码错误，请重新输入");
	}
}
int main(int argc, char* argv[])
{
	//登陆窗体的设置和初始化
	GtkWidget* window;
	gtk_init(&argc, &argv);//初始化
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"登录窗口");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window),200,100);
	gtk_container_set_border_width(GTK_CONTAINER(window),20);
	//设置窗口布局
	GtkWidget* box;
	GtkWidget* box1;
	GtkWidget* box2;
	GtkWidget* sep;
	box = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),box);
	box1 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),box1,FALSE,FALSE,5);
	box2 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(box),box2,FALSE,FALSE,5);
	sep = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(box),sep,FALSE,FALSE,5);
	//用户名和密码
	GtkWidget* label1;
	GtkWidget* label2;
	label1 = gtk_label_new("用户名：");
	entry1 = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(box1),label1,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box1),entry1,FALSE,FALSE,5);
	label2 = gtk_label_new("密码：     ");
	entry2 = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(entry2),FALSE);
	gtk_box_pack_start(GTK_BOX(box2),label2,FALSE,FALSE,5);
	gtk_box_pack_start(GTK_BOX(box2),entry2,FALSE,FALSE,5);
	//添加提示信息
	GtkWidget  *viewport;
	viewport = gtk_viewport_new(NULL,NULL);
	gtk_box_pack_start(GTK_BOX(box),viewport,FALSE,FALSE,5);
	blabel = gtk_label_new(NULL);
	gtk_container_add(GTK_CONTAINER(viewport),blabel);
	//添加确认按钮
	GtkWidget* button;
	button = gtk_button_new_with_label("确认");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_button_clicked),NULL);
	gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);
	gtk_widget_show_all(window);
	gtk_main();
	return FALSE;
}
