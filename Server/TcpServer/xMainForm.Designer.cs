namespace xProtectorServer
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.button1 = new System.Windows.Forms.Button();
            this.lstConectados = new System.Windows.Forms.ListBox();
            this.txtCommand = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.txtServerLog = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.getHardware = new System.Windows.Forms.Button();
            this.startsniff = new System.Windows.Forms.Button();
            this.StopSoftwareSniff = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(752, 506);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(70, 27);
            this.button1.TabIndex = 0;
            this.button1.Text = "Send";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // lstConectados
            // 
            this.lstConectados.FormattingEnabled = true;
            this.lstConectados.Location = new System.Drawing.Point(383, 411);
            this.lstConectados.Name = "lstConectados";
            this.lstConectados.Size = new System.Drawing.Size(439, 69);
            this.lstConectados.TabIndex = 1;
            // 
            // txtCommand
            // 
            this.txtCommand.Location = new System.Drawing.Point(382, 513);
            this.txtCommand.Name = "txtCommand";
            this.txtCommand.Size = new System.Drawing.Size(364, 20);
            this.txtCommand.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(379, 497);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(92, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Command to send";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(380, 395);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(96, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Connected Clients:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(32, 411);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(278, 104);
            this.label4.TabIndex = 7;
            this.label4.Text = resources.GetString("label4.Text");
            // 
            // txtServerLog
            // 
            this.txtServerLog.Location = new System.Drawing.Point(12, 25);
            this.txtServerLog.Multiline = true;
            this.txtServerLog.Name = "txtServerLog";
            this.txtServerLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtServerLog.Size = new System.Drawing.Size(829, 354);
            this.txtServerLog.TabIndex = 8;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 9);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(59, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Server Log";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 386);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(61, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "Instructions";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // getHardware
            // 
            this.getHardware.Location = new System.Drawing.Point(15, 576);
            this.getHardware.Name = "getHardware";
            this.getHardware.Size = new System.Drawing.Size(102, 23);
            this.getHardware.TabIndex = 11;
            this.getHardware.Text = "GetHardwareInfo";
            this.getHardware.UseVisualStyleBackColor = true;
            this.getHardware.Click += new System.EventHandler(this.getHardware_Click);
            // 
            // startsniff
            // 
            this.startsniff.Cursor = System.Windows.Forms.Cursors.Default;
            this.startsniff.Location = new System.Drawing.Point(139, 576);
            this.startsniff.Name = "startsniff";
            this.startsniff.Size = new System.Drawing.Size(108, 23);
            this.startsniff.TabIndex = 12;
            this.startsniff.Text = "StartSoftwareSniff";
            this.startsniff.UseVisualStyleBackColor = true;
            this.startsniff.Click += new System.EventHandler(this.startsniff_Click);
            // 
            // StopSoftwareSniff
            // 
            this.StopSoftwareSniff.Location = new System.Drawing.Point(272, 576);
            this.StopSoftwareSniff.Name = "StopSoftwareSniff";
            this.StopSoftwareSniff.Size = new System.Drawing.Size(102, 23);
            this.StopSoftwareSniff.TabIndex = 13;
            this.StopSoftwareSniff.Text = "StopSoftwareSniff";
            this.StopSoftwareSniff.UseVisualStyleBackColor = true;
            this.StopSoftwareSniff.Click += new System.EventHandler(this.StopSoftwareSniff_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(856, 649);
            this.Controls.Add(this.StopSoftwareSniff);
            this.Controls.Add(this.startsniff);
            this.Controls.Add(this.getHardware);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.txtServerLog);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtCommand);
            this.Controls.Add(this.lstConectados);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "xController Server Tester";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ListBox lstConectados;
        private System.Windows.Forms.TextBox txtCommand;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtServerLog;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button getHardware;
        private System.Windows.Forms.Button startsniff;
        private System.Windows.Forms.Button StopSoftwareSniff;
    }
}

