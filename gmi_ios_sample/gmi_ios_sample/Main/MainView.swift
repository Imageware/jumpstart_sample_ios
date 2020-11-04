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
        
        Text("Hello, world!")
            .padding()
    }
}

