/* @TODO - Awesomizer needs to support a context and this integration needs
         to accept a function which can generate that context from the request
         object.
   */
let make = (~schema, ~decoder, ~encoder, ~handler) => {
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
  let awesomize = Awesomize.make(schema);
  (_, req, res) =>
    Express.Request.asJsonObject(req)
    |> awesomize
    |> Awesomize.Result.fold(
         err =>
           res
           |> Express.Response.status(Express.Response.StatusCode.BadRequest)
           |> Express.Response.sendJson(err |> errorMapToJson)
           |> Js.Promise.resolve,
         data =>
           data
           |> beltMapToJson
           |> decoder
           |> handler(req)
           |> Js.Promise.then_(json =>
                res
                |> Express.Response.status(Express.Response.StatusCode.Ok)
                |> Express.Response.sendJson(json |> encoder)
                |> Js.Promise.resolve
              ),
       )
    |> Js.Promise.then_(result => result);
};
