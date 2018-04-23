/* Private Functions */
let badRequest = Express.Response.StatusCode.BadRequest;

let ok = Express.Response.StatusCode.Ok;

let beltMapToJson = map =>
  Belt.Map.String.toArray(map) |> Js.Dict.fromArray |> Js.Json.object_;

let errorMapToJson = map =>
  Belt.Map.String.map(map, x =>
    switch (x) {
    | None => Js.Json.null
    | Some(str) => Js.Json.string(str)
    }
  )
  |> beltMapToJson;

let sendJson = (res, statusCode, json) =>
  res
  |> Express.Response.status(statusCode)
  |> Express.Response.sendJson(json)
  |> Js.Promise.resolve;

/* Public Functions */
/* @TODO - Awesomizer needs to support a context and this integration needs
   to accept a function which can generate that context from the request
   object. */
let make = (~schema, ~decoder, ~encoder, ~handler) => {
  let awesomize = Awesomize.make(schema);
  (_, req, res) =>
    Express.Request.asJsonObject(req)
    |> awesomize
    |> Awesomize.Result.fold(
         err => sendJson(res, badRequest, errorMapToJson(err)),
         data =>
           data
           |> beltMapToJson
           |> decoder
           |> handler(req)
           |> Js.Promise.then_(json => sendJson(res, ok, encoder(json))),
       )
    |> Js.Promise.then_(result => result);
};
