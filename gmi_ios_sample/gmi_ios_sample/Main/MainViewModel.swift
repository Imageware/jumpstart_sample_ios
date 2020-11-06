//
//  MainViewModel.swift
//  gmi_ios_sample
//
//  Created by Vitalii Poponov on 11/4/20.
//
import SwiftUI
import Foundation
import GMI

class MainViewModel: ObservableObject {
    
    private var deviceServiceManager: DeviceServiceManagerProtocol = DeviceServiceManager()
    private var messagesManager: MessagesServiceManagerProtocol = MessagesServiceManager()
    @Published var showingAlert = false
    var alertTitle: String = ""
    
    @Published var serverURL: String = "https://eks-gmiserver-dev.iwsinc.com"
    @Published var userManagerURL: String = "https://eks-usermanager-dev.iwsinc.com"
    
    @Published var clientID: String = "GoVerifyID"
    @Published var clientSecret: String = "Un9#He!#TshDmv/Z"
    
    @Published var tenantCode: String = "ImageWareRefresh"
    @Published var applicationCode: String = "GoVerifyID"
    
    @Published var email: String = ""
    
    var config: Config {
        return Config(presetName: "", presetTitle: "", serverUrl: self.serverURL, clientSecret: self.clientSecret, clientID: self.clientID, userManagerUrl: self.userManagerURL, defaultTenantCode: self.tenantCode, applicationCode: self.applicationCode)
    }
    var account: Account?
    
    func initializeSDK() {
        try? Keychain().generateAndStoreNewKey()
        self.alertTitle = "SDK Initialized"
        self.showingAlert.toggle()
    }
    func checkEmail() {
       
        let account = Account()
        account.config = config
        account.email = email
        deviceServiceManager.personForEmail(account: account) { response in
            switch response {
            case .success(let person):
                account.id = person.id
                account.tenantEnrollServer = person.tenantEnrollServer
                account.enrollServer = person.enrollServer
                account.tenantVerifyServer = person.tenantVerifyServer
                account.verifyServer = person.verifyServer
                self.account = account
                self.alertTitle = "Email \(self.email) successully found"
                self.showingAlert.toggle()
            case .error:
                self.alertTitle = "Email not found in the selected Region"
                self.showingAlert.toggle()
            @unknown default: break
            }
        }
    }
    
    func registerUser() {
        guard let account = self.account else { return }
        deviceServiceManager.registerDevice(account: account, persistUser: true) { [weak self] (response) in
            switch response {
            case .success:
                self?.alertTitle = "Confirm your email. You should receive email"
                self?.showingAlert.toggle()
            case .error(let error):
                if case .notFound = error {//this is unexpected because we just checked before eula
                    self?.alertTitle = "Email not found in the selected Region"
                } else { //if we don't get a 200, something is broken that the user can't do anything about
                    self?.alertTitle = "Server Error"
                }
                self?.showingAlert.toggle()
            @unknown default: break
            }
        }
    }
    
    func syncronize() {
        if let vc = (UIApplication.shared.connectedScenes.first?.delegate as? SceneDelegate)?.window?.rootViewController {
            Instance.shared.interactionManager.register(viewController: vc, delegate: self)
        }
        messagesManager.syncronizeAlertsAndEnrollments {
            if Instance.shared.captureFlowManager.isOkayToBeginCaptureFlow() {
                if let enroll = Instance.shared.captureFlowManager.nextExpectedEnrollment(),
                    let enrollCaptureFlow = CaptureFlow(enroll) {
                    Instance.shared.captureFlowManager.beginCaptureFlow(captureFlow: enrollCaptureFlow)
                } else if let alert = Instance.shared.captureFlowManager.nextUnreadQueueItem(),
                    let alertCaptureFlow = CaptureFlow(alert) {
                    Instance.shared.captureFlowManager.beginCaptureFlow(captureFlow: alertCaptureFlow)
                }
                else {
                    self.alertTitle = "There is no pending enrolls or alerts"
                    self.showingAlert.toggle()
                }
            }
        }
    }
    
    func countPendingAlertsAndEnrolls() {
        self.alertTitle = "There are \(messagesManager.activeAlerts) pending alerts and \(messagesManager.activeEnrolls) enrolls"
        self.showingAlert.toggle()
    }
}


extension MainViewModel: InteractionManagerDelegate {
    
    public func alertCompleted(_ captureable: Capturable) {
        self.alertTitle = "alert completed"
        self.showingAlert.toggle()
    }
    
    public func alertAccepted(_ captureable: Capturable) {
        self.alertTitle = "alert accepted"
        self.showingAlert.toggle()
    }
    
    public func alertRejected(_ captureable: Capturable) {
        self.alertTitle = "alert rejected"
        self.showingAlert.toggle()
    }
    
    public func enrollmentHidden(_ captureable: Capturable) {
        self.alertTitle = "enrollment hidden"
        self.showingAlert.toggle()
    }
    
    public func enrollmentComplete(_ captureable: Capturable) {
        self.alertTitle = "enrollment complete"
        self.showingAlert.toggle()
    }
}
