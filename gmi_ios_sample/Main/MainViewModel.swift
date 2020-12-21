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
    
    private var deviceServiceManager: AccountServiceManagerProtocol = AccountServiceManager()
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
    
    var config: Configuration {
        return Configuration(presetTitle: "", serverUrl: self.serverURL, clientSecret: self.clientSecret, clientID: self.clientID, userManagerUrl: self.userManagerURL, defaultTenantCode: self.tenantCode, applicationCode: self.applicationCode)
    }
    
    var account: Profile?
    
    
    func registerUser() {
        let profile = Profile(name: "", email: email, configuration: config)

        deviceServiceManager.register(profile: profile, resendConfirmation: true) { (response) in
            switch response {
            case .success(let status):
                switch status {
                case .pendingVerification:
                    self.alertTitle = "Check your email to confirm registration"
                    self.showingAlert.toggle()
                case .userVerified:
                    self.alertTitle = "Account registration successfully completed"
                    self.showingAlert.toggle()
                }
            case .error(let error):
                switch error {
                case .notFound:
                    self.alertTitle = "Email not found in the selected Region"
                default:
                    self.alertTitle = "Server Error"
                }
                self.showingAlert.toggle()
            }
        }
    }
    
    func syncronize() {
        if let vc = (UIApplication.shared.connectedScenes.first?.delegate as? SceneDelegate)?.window?.rootViewController {
            messagesManager.register(rootController: vc, delegate: self)
        }
        messagesManager.synchronizeWorkItems {
            self.messagesManager.renderNextWorkItemIfNeeded()
        }
    }
    
    func countPendingAlertsAndEnrolls() {
        self.alertTitle = "There are \(messagesManager.activeAlertsCount) pending alerts and \(messagesManager.activeEnrollsCount) enrolls"
        self.showingAlert.toggle()
    }
}


extension MainViewModel: InteractionManagerDelegate {
    
    public func alertCompleted() {
        self.alertTitle = "alert completed"
        self.showingAlert.toggle()
    }
    
    public func alertAccepted() {
        self.alertTitle = "alert accepted"
        self.showingAlert.toggle()
    }
    
    public func alertRejected() {
        self.alertTitle = "alert rejected"
        self.showingAlert.toggle()
    }
    
    public func enrollmentHidden() {
        self.alertTitle = "enrollment hidden"
        self.showingAlert.toggle()
    }
    
    public func enrollmentComplete() {
        self.alertTitle = "enrollment complete"
        self.showingAlert.toggle()
    }
}
