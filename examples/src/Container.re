module RList = Rationale.RList;
open Belt;

type action =
  | MoveCard(int, int);

type state = {cards: list(T.card)};

let component = ReasonReact.reducerComponent("Container");

let make = _children => {
  ...component,
  initialState: () => {
    cards: [
      {id: 1, text: "Write a cool JS library"},
      {id: 2, text: "Make it generic enough"},
      {id: 3, text: "Write README"},
      {id: 4, text: "Create some examples"},
      {id: 5, text: "Spam in Twitter and IRC to promote it"},
      {id: 6, text: "???"},
      {id: 7, text: "PROFIT"},
    ],
  },
  reducer: (action, state) =>
    switch (action) {
    | MoveCard(dragId, hoverId) =>
      let dragIndex =
        RList.findIndex((card: T.card) => card.id === dragId, state.cards);
      let hoverIndex =
        RList.findIndex((card: T.card) => card.id === hoverId, state.cards);
      Js.log3("ID:", dragId, hoverId);
      switch (dragIndex, hoverIndex) {
      | (Some(dragIndex), Some(hoverIndex)) =>
        let card = List.getExn(state.cards, dragIndex);
        ReasonReact.Update({
          cards:
            state.cards
            ->RList.remove(dragIndex, 1, _)
            ->RList.insert(hoverIndex, card, _),
        });
      | (_, _) => ReasonReact.NoUpdate
      };
    },
  render: self => {
    <BsReactDnd.DragDropContextProvider backend=BsReactDnd.Backend.html5>
      <div style={ReactDOMRe.Style.make(~width="400", ())}>
        self.state.cards
        ->(
            List.map((card: T.card) =>
              <Card
                key={string_of_int(card.id)}
                id={card.id}
                text={card.text}
                moveCard={
                  (dragIndex, hoverIndex) =>
                    self.send(MoveCard(dragIndex, hoverIndex))
                }
              />
            )
          )
        ->List.toArray
        ->ReasonReact.array
      </div>
    </BsReactDnd.DragDropContextProvider>;
  },
};
