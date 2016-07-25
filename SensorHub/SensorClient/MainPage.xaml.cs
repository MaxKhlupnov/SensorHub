using System;
using Windows.Foundation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;


using System.Threading;
using System.Threading.Tasks;


using SensorClient.DataModel;
using SensorClient.Common;

using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Configurations;
using SensorClient.Devices;

using WinRTXamlToolkit.Debugging;
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace SensorClient
{


    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        static CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

        /* Device */          
        public MultisensorViewModel devicesViewModel = null;
        public ZWaveDeviceManager deviceManager = null;
        private readonly IConfigurationProvider _configProvider = new ConfigurationProvider();
        TextBox txtDeviceId;
        private TraceLogger _logger = new TraceLogger();
        public MainPage()
        {
            DC.ShowLog();
           // this.devicesViewModel = new MultisensorViewModel(_logger, _configProvider);
            this.deviceManager = new ZWaveDeviceManager(_configProvider, _logger, cancellationTokenSource.Token);

            //this.DataContext = this.devicesViewModel;
            this.InitializeComponent();
  
            //((SensorClient.App)Application.Current).OnBridgeInitialized += OnBridgeInitialized;
            this.Loaded += MainPage_Loaded;            
        }

        private void MainPage_Loaded(object sender, RoutedEventArgs e)
        {            
           // DC.Hide();
            
        //    Task.Run(() => devicesViewModel.ProcessDevicesAsync(cancellationTokenSource.Token), cancellationTokenSource.Token);
        }
        
        private void OnBridgeInitialized(IAsyncAction asyncAction, AsyncStatus asyncStatus)
        {
            _logger.LogInfo("AllJoyn bridge successfully activated");
        }

        /// <summary>
        /// Invoked when a HubSection header is clicked.
        /// </summary>
        /// <param name="sender">The Hub that contains the HubSection whose header was clicked.</param>
        /// <param name="e">Event data that describes how the click was initiated.</param>
        void Hub_SectionHeaderClick(object sender, HubSectionHeaderClickEventArgs e)
        {
            HubSection section = e.Section;
            /*var group = section.DataContext;
            this.Frame.Navigate(typeof(SectionPage), ((ControlInfoDataGroup)group).UniqueId);*/
        }

        /// <summary>
        /// Invoked when an item within a section is clicked.
        /// </summary>
        /// <param name="sender">The GridView or ListView
        /// displaying the item clicked.</param>
        /// <param name="e">Event data that describes the item clicked.</param>
        void SensorsView_ItemClick(object sender, ItemClickEventArgs e)
        {
            // Navigate to the appropriate destination page, configuring the new page
            // by passing required information as a navigation parameter
            /*var itemId = ((ControlInfoDataItem)e.ClickedItem).UniqueId;
            this.Frame.Navigate(typeof(ItemPage), itemId);*/
        }

      

        private void DebugPopup_Closed(object sender, object e)
        {
            if (chkShowTracePanel != null)
                chkShowTracePanel.IsChecked = false;
        }

        CheckBox chkShowTracePanel = null;
        private void chkShowTracePanel_Loaded(object sender, RoutedEventArgs e)
        {
            chkShowTracePanel = sender as CheckBox;
        }

        /// <summary>
        ///  Add demo device for UI testing purposes
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnAddDevice_Click(object sender, RoutedEventArgs e)
        {
            
           /* dynamic testDevice = SensorClient.Devices.AllJoynDeviceFactory.CreateDummyDevice("000-000");
            TemperatureSensor sensor = new TemperatureSensor(null, "Dummy");
            this.devicesViewModel.SensorStarted(sensor, testDevice);*/

        }

        private void button_SetConfigurationClick(object sender, RoutedEventArgs e)
        {
            if (this.deviceManager != null)
                this.deviceManager.SetDefaultConfiguration();
        }
    }


}
