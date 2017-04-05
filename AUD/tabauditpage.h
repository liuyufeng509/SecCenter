#ifndef TABAUDITPAGE_H
#define TABAUDITPAGE_H

#include <QWidget>
#include"common.h"
namespace Ui {
class TabAuditPage;
}

enum OP_TYPE
{
    OPEN_CLOSE_AUD,
    QUERY_SET,
    AUD_REPORT,
    KER_AUD_PARM,
    FILE_AUD_PARM,
    SYS_CALL_RULE,
    CUSTOM_RULE,
    AUD_CONFIG,
    AUD_DISPLAY

};

class TabAuditPage : public QWidget
{
    Q_OBJECT
signals:
 //   void emitQuerySentence(QString cmd);
public:
    explicit TabAuditPage(QWidget *parent = 0);
    ~TabAuditPage();
    void set_signal_slot();
    bool check_param();
    void update_kern_aud_param_ui();
    void save_kern_aud_param_from_ui();

    void get_aud_config_info();
    void update_aud_config_ui();
    void save_aud_config_from_ui();
    //void set_config_content(QString key, QString value);
    void read_and_display_file();

    void save_file_rules_from_ui();


private slots:
//    void on_openButton_clicked();

//    void on_queryButton_clicked();

//    void on_reportButton_clicked();

//    void on_roleButton_clicked();

//    void on_setCfgButton_clicked();

    void on_closeAduButton_clicked();
    void set_query_fun_ui();

    void on_query_produceButton_clicked();

    void on_aplButton_clicked();

    void on_sv_to_fileButton_clicked();

    void on_res_checkBox_clicked();

    void on_tm_sec_checkBox_clicked();

    void on_report_okButton_clicked();

    void on_apl_kern_paramButton_clicked();

    void on_ker_aud_param_saveButton_clicked();

    void on_apl_cfg_Button_clicked();

    void on_open_pushButton_clicked();

    void on_fresh_pushButton_clicked();

    void on_pre_pushButton_clicked();

    void on_next_pushButton_clicked();

    void on_file_rule_aply_pushButton_clicked();

    void on_brow_pushButton_clicked();

    void on_file_aud_param_saveButton_clicked();

    void on_trace_Button_clicked();

    void on_list_group_radioButton_clicked();

    void on_sys_call_rule_apl_pushButton_clicked();
    
    void on_sys_call_save_pushButton_clicked();
    
    void on_clean_all_rules_Button_clicked();

    void on_clean_all_rules_infile_Button_clicked();

    void on_display_cur_rules_Button_clicked();

private:
    Ui::TabAuditPage *ui;
    QString cmd;
    QString res;

    KernAudParam kernAudParam;
    AudConfigInfo audCfgInfo;

    QString fileName;
    QMap<QString, QString> config_settings;
    OP_TYPE op_type;

    FileAudRule fileRule;
};

#endif // TABAUDITPAGE_H
