/* DragSource
 * https://react-dnd.github.io/react-dnd/docs-drag-source.html
 */
open Core;

module MakeSpec = (Config: {type props; type dndItem;}) => {
  type monitor = {
    .
    "isDragging": [@bs.meth] (unit => Js.boolean),
    "getItem": [@bs.meth] (unit => Js.nullable(Config.dndItem)),
    "getDropResult": [@bs.meth] (unit => Js.nullable(Config.dndItem))
  };
  type t = {
    .
    "beginDrag":
      (Config.props, monitor, ReasonReact.reactRef) => Config.dndItem,
    "endDrag": (Config.props, monitor, ReasonReact.reactRef) => unit,
    "canDrag": (Config.props, monitor) => bool,
    "isDragging": (Config.props, monitor) => bool
  };
  [@bs.obj]
  external make :
    (
      ~beginDrag: (Config.props, monitor, ReasonReact.reactRef) =>
                  Config.dndItem
                    =?,
      ~endDrag: (Config.props, monitor, ReasonReact.reactRef) => unit=?,
      ~canDrag: (Config.props, monitor) => bool=?,
      ~isDragging: (Config.props, monitor) => bool=?,
      unit
    ) =>
    t =
    "";
};

module type MakeConfig = {
  type spec;
  type collectedProps;
  type collect;
  let itemType: string;
  let spec: spec;
  let collect: collect;
};

module Make = (Config: MakeConfig) => {
  external convertToCollectedProps : Js.t({..}) => Config.collectedProps =
    "%identity";
  [@bs.module "react-dnd"]
  external dragSource : (string, Config.spec, Config.collect) => hoc =
    "DragSource";
  type children =
    (~collectedProps: Config.collectedProps) => ReasonReact.reactElement;
  let component = ReasonReact.statelessComponent("DragSource");
  let make' = (~collectedProps: Config.collectedProps, children: children) => {
    ...component,
    render: _self => children(~collectedProps)
  };
  /* Convert Reason => JS */
  let jsComponent =
    ReasonReact.wrapReasonForJs(~component, (props: {. "children": children}) =>
      make'(~collectedProps=convertToCollectedProps(props), props##children)
    );
  /* Wrap JS class with 'react-dnd' */
  let enhanced =
    dragSource(Config.itemType, Config.spec, Config.collect, jsComponent);
  /* Convert JS => Reason */
  let make = (~props=Js.Obj.empty(), children: children) =>
    ReasonReact.wrapJsForReason(~reactClass=enhanced, ~props, children);
};