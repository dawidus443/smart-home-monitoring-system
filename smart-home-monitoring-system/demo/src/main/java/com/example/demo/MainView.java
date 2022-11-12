package com.example.demo;

import com.vaadin.flow.component.Component;
import com.vaadin.flow.component.Key;
import com.vaadin.flow.component.button.Button;
import com.vaadin.flow.component.button.ButtonVariant;
import com.vaadin.flow.component.grid.Grid;
import com.vaadin.flow.component.orderedlayout.HorizontalLayout;
import com.vaadin.flow.component.orderedlayout.VerticalLayout;
import com.vaadin.flow.component.textfield.TextField;
import com.vaadin.flow.data.binder.Binder;
import com.vaadin.flow.data.binder.ValidationException;
import com.vaadin.flow.router.Route;

import java.awt.*;

@Route("")
public class MainView extends VerticalLayout {
    private RoomRepository repository;
    private TextField roomType = new TextField("Typ pokoju");
    private TextField roomArea = new TextField("Powierzchnia pokoju");
    private Grid<Room> grid = new Grid<>(Room.class);
    private Binder<Room> binder = new Binder<>(Room.class);


    public MainView(RoomRepository repository){
        this.repository = repository;

        grid.setColumns("roomType", "roomArea");

        add(getForm(),grid);
        refreshGrid();

    }

    private Component getForm(){
        var layout = new HorizontalLayout();
        layout.setAlignItems(Alignment.BASELINE);

        var addButton = new Button("Dodaj");
        addButton.addClickShortcut(Key.ENTER);
        addButton.addThemeVariants(ButtonVariant.LUMO_PRIMARY);
        layout.add(roomType, roomArea, addButton);

        binder.bindInstanceFields(this);

        addButton.addClickListener(click -> {
            try{
                var room = new Room();
                binder.writeBean(room);
                repository.save((room));
                binder.readBean(new Room());
                refreshGrid();
            }catch (ValidationException e){

            }
        });

        return layout;
    }

    private void refreshGrid(){
        grid.setItems(repository.findAll());
    }
}
