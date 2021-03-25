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
    
    @Published var serverURL: String = "https://gmi.prod.imageware.io/v2/gmiserver"
    @Published var userManagerURL: String =    "https://gmi.prod.imageware.io/v1/oauth2"
    
    @Published var clientID: String = "GoVerifyID"
    @Published var clientSecret: String = "73rOgqah&PVD"
    
    @Published var tenantCode: String = "ImageWare"
    @Published var applicationCode: String = "GoVerifyID"
    
    @Published var email: String = ""
    
    @Published var pincode: String = ""
    @Published var isRegistered: Bool = false
    
    var config: Configuration {
        return Configuration(presetTitle: "", serverUrl: self.serverURL, clientSecret: self.clientSecret, clientID: self.clientID, userManagerUrl: self.userManagerURL, defaultTenantCode: self.tenantCode, applicationCode: self.applicationCode)
    }
    
    var account: Profile?
    
    func validateTenant() {
        guard let profile = account else { return }
        deviceServiceManager.validate(profile: profile, validationCode: self.pincode) { (response) in
            switch response {
            case .success:
                self.alertTitle = "Successully added a new tenant"
                self.showingAlert.toggle()
                self.pincode = ""
            case .error:
                self.alertTitle = "There was an error adding a new tenant. Check the correctness of the PIN and try again."
                self.showingAlert.toggle()
                self.pincode = ""
            }
        }
    }
    func registerUser() {
        let profile = Profile(name: "", email: email, configuration: config)

        deviceServiceManager.register(profile: profile) { (response) in
            switch response {
            case .success(let profile):
                self.account = profile
                self.alertTitle = "Check your email. Validate tenants you want to add"
                self.showingAlert.toggle()
                self.isRegistered = true
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
