//
//  MainView.swift
//  gmi_ios_sample
//
//  Created by Vitalii Poponov on 11/3/20.
//

import SwiftUI

struct MainView: View {
    
    
    @ObservedObject var viewModel: MainViewModel
    
    init(viewModel: MainViewModel = MainViewModel()) {
        self.viewModel = viewModel
        
    }
    
    var body: some View {
        Form {

            Section(header: Text("GMI Server URL")) {
                TextField("GMI Server URL", text: self.$viewModel.serverURL).disabled(viewModel.isRegistered)
                TextField("GMI UserManager URL", text: self.$viewModel.userManagerURL).disabled(viewModel.isRegistered)
            }

            Section(header: Text("Client info")) {
                TextField("GMI Client ID", text: self.$viewModel.clientID).disabled(viewModel.isRegistered)
                TextField("GMI Client secret", text: self.$viewModel.clientSecret).disabled(viewModel.isRegistered)
            }

            Section(header: Text("")) {
                TextField("GMI Tenant Code", text: self.$viewModel.tenantCode).disabled(viewModel.isRegistered)
                TextField("GMI Application Code", text: self.$viewModel.applicationCode).disabled(viewModel.isRegistered)
            }
            
            Section(header: Text("Email / User ID")) {
                TextField("email", text: self.$viewModel.email).disabled(viewModel.isRegistered)
                    .keyboardType(.emailAddress).disableAutocorrection(true).autocapitalization(.none)
            }
            if viewModel.isRegistered {
                Section(header: Text("Tenant validation")) {
                    TextField("PIN code", text: self.$viewModel.pincode)
                    Button(action: { viewModel.validateTenant() }) { Text("Validate tenant") }
                }
                
                Button(action: { viewModel.syncronize() }) { Text("Syncronize and run first expected item") }
                Button(action: { viewModel.countPendingAlertsAndEnrolls() }) { Text("Count of pending alerts and enrolls") }
            }
            else {
                Button(action: { viewModel.registerUser() }) { Text("Register User") }
            }
            
            
        }.alert(isPresented: self.$viewModel.showingAlert) {
            Alert(title: Text("Message"), message: Text(self.viewModel.alertTitle), dismissButton: .default(Text("Dismiss")))
        }
    }
}

