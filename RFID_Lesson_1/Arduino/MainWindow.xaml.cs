using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;
using WindowsInput;
using WindowsInput.Native;

namespace Arduino
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        InputSimulator simulator = new InputSimulator();
        SerialPort sp = new SerialPort();
        string[] ports = SerialPort.GetPortNames();

        public MainWindow()
        {
            InitializeComponent();
            COM.ItemsSource = ports;
            sp.DataReceived += new SerialDataReceivedEventHandler(DataReceived);
        }

        
        private void COM_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                if (sp.IsOpen)
                    sp.Close();
                sp.PortName = COM.SelectedItem as string;
                sp.BaudRate = 9600;
                sp.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        void DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string uid = default;
            Dispatcher.Invoke(() => uid = sp.ReadExisting());
            switch (uid)
            {
                case "f976ed6e":
                    simulator.Keyboard.TextEntry("VasiliyPupkin");
                    simulator.Keyboard.KeyPress(VirtualKeyCode.TAB);
                    simulator.Keyboard.TextEntry("Пароль");
                    simulator.Keyboard.KeyPress(VirtualKeyCode.RETURN);
                    break;
                case "19fcda6e":
                    simulator.Keyboard.TextEntry("Arseny@seny.gom");
                    simulator.Keyboard.KeyPress(VirtualKeyCode.TAB);
                    simulator.Keyboard.TextEntry("12345qwerty");
                    simulator.Keyboard.KeyPress(VirtualKeyCode.RETURN);
                    break;
            }
        }
    }
}
